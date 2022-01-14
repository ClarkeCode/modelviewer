<?php
const ECHO_DEBUG = FALSE;

//NOTE: Parameter 'conn' is an outparam
//Php will exit if it is unable to connect to the specified database
function establish_db_connection($server, $username, $password, $database, &$conn) {
	$verification = new mysqli($server, $username, $password, "information_schema");
	if ($verification->connect_error) {
		die("Was unable to determine if database '".$database."' must be created: " . $verification->connect_error);
	}
	$result = mysqli_query($verification, "select * from schemata where SCHEMA_NAME like '".$database."'");
	if (mysqli_num_rows($result) == 0) {
		if ($verification->query("create database ".$database) !== TRUE) {
			die("Database '".$database."' does not exist and could not be created: " . $verification->connect_error);
		}
		if (ECHO_DEBUG) {echo "Created database ".$database." <BR>";}
	}
	$conn = new mysqli($server, $username, $password, $database);
	if ($conn->connect_error) {
		die("Could not connect to database '".$database."'" . $conn->connect_error);
	}
	if (ECHO_DEBUG) {echo "Connected successfully at: ".$database." <BR>";}
}

function db_execute_queries($connection, $queries, $doecho) {
	foreach ($queries as $query) {
		if ($connection->query($query) === TRUE) {
			if ($doecho) {echo "Success: ".$query." <BR>";}
		}
		else {
			if ($doecho) {echo "Failure: ".$query." <BR>";}
		}
	}
}


function db_create_all_tables($connection) {
	$queries = array();
	array_push($queries,
		"create table model(uid int not null auto_increment, 
			name char(255),
			sequence_length int,
			ppm_height int,
			ppm_width int,
			primary key(uid))");
	array_push($queries,
		"create table vertex(uid int not null auto_increment, 
			model_file_id int,
			x float,
			y float,
			z float,
			primary key(uid))");
	array_push($queries,
		"create table normal(uid int not null auto_increment, 
			model_file_id int,
			x float,
			y float,
			z float,
			primary key(uid))");
	array_push($queries,
		"create table texture(uid int not null auto_increment, 
			model_file_id int,
			x float,
			y float,
			primary key(uid))");
	array_push($queries,
		"create table ppm(uid int not null auto_increment, 
			model_file_id int,
			r int,
			g int,
			b int,
			a int,
			primary key(uid))");
	
	//NOTE: If a table already exists, the query fails and does not overwrite table
	db_execute_queries($connection, $queries, ECHO_DEBUG);
}

function db_drop_all_tables($connection) {
	$queries = array("drop table vertex");
	array_push($queries, "drop table normal");
	array_push($queries, "drop table texture");
	array_push($queries, "drop table ppm");
	array_push($queries, "drop table model");

	db_execute_queries($connection, $queries, ECHO_DEBUG);
}

function db_insert_JSON_data($connection, $jstr, $modelname) {
	//See if there is already information for the specified model
	$result = mysqli_query($connection, "select uid, sequence_length, ppm_height from model where name = '".$modelname."'");
	if (mysqli_num_rows($result) == 0) {
		$result =  mysqli_query($connection, "insert into model (name) values ('".$modelname."')");
		if ($result === FALSE) {
			die("ERROR: Unable to add ".$modelname."to database");
		}
		$result = mysqli_query($connection, "select uid, sequence_length, ppm_height from model where name = '".$modelname."'");
	}
	$row = mysqli_fetch_assoc($result);
	$fileid = $row["uid"];
	$db_missing_obj = $row["sequence_length"] == NULL;
	$db_missing_ppm = $row["ppm_height"] == NULL;

	
	$jso = json_decode($jstr, true);
	$has_obj_data = $jso["vertexCount"] != 0;
	$has_ppm_data = $jso["width"] != 0;
	
	if (ECHO_DEBUG) {echo sprintf("Aquired %s as #%d, OBJ: %d PPM: %d<br>", $modelname, $fileid, $has_obj_data, $has_ppm_data);}

	$queries = array();

	if ($has_obj_data && $db_missing_obj) {
		$vertex_count = $jso["vertexCount"];

		array_push($queries, sprintf("update model set sequence_length = %d where name = '%s'", $vertex_count, $modelname));
		$verticies = $jso["verticies"];
		
		$astr = array();
		foreach (range(0, $vertex_count*3-1, 3) as $x) {
			array_push($astr, sprintf("(%d, %f, %f, %f)",
			$fileid, $verticies[$x], $verticies[$x+1], $verticies[$x+2]));
		}
		array_push($queries, sprintf("insert into vertex (model_file_id, x, y, z) values%s", implode(",", $astr)));
		
		$astr = array();
		foreach (range(0, $vertex_count*3-1, 3) as $x) {
			array_push($astr, sprintf("(%d, %f, %f, %f)",
			$fileid, $jso["normals"][$x], $jso["normals"][$x+1], $jso["normals"][$x+2]));
		}
		array_push($queries, sprintf("insert into normal (model_file_id, x, y, z) values%s", implode(",", $astr)));
		
		$astr = array();
		foreach (range(0, $vertex_count*2-1, 2) as $x) {
			array_push($astr, sprintf("(%d, %f, %f)",
				$fileid, $jso["textcoords"][$x], $jso["textcoords"][$x+1]));
		}
		array_push($queries, sprintf("insert into texture (model_file_id, x, y) values%s", implode(",", $astr)));
	}

	if ($has_ppm_data && $db_missing_ppm) {
		array_push($queries, sprintf("update model set ppm_width = %d where name = '%s'", $jso["width"], $modelname));
		array_push($queries, sprintf("update model set ppm_height = %d where name = '%s'", $jso["height"], $modelname));

		$astr = array();
		foreach (range(0, count($jso["rgba"])-1, 4) as $x) {
			array_push($astr, sprintf("(%d, %d, %d, %d, %d)",
				$fileid, $jso["rgba"][$x], $jso["rgba"][$x+1], $jso["rgba"][$x+2], $jso["rgba"][$x+3]));
		}

		array_push($queries, sprintf("insert into ppm (model_file_id, r, g, b, a) values%s", implode(",", $astr)));
	}

	db_execute_queries($connection, $queries, ECHO_DEBUG && FALSE);
}

const MAX_BUTTONS_SHOWN = 20;
const BUTTONS_PER_ROW = 3;

function db_generate_display_buttons($connection) {
	$result = mysqli_query($connection, "select uid, name from model where sequence_length is not null and ppm_height is not null");
	if ($result === FALSE) return;
	echo "<table><tbody><tr>";
	for ($button=0; $row = mysqli_fetch_assoc($result); $button++) {
		echo "<td><form method=\"post\" action=\"index.php\" enctype=\"multipart/form-data\" style=\"display: inline-block; width: 100%;\">";
		echo sprintf("<button style=\"width: 100%%;\" name=\"displayFromDB\" value=%d>Display #%d '%s'</button>", $row["uid"], $row["uid"], $row["name"]);
		echo "</form></td>";

		if (($button+1) % BUTTONS_PER_ROW == 0 && $button != 0) {
			echo "</tr><tr>";
		}

		if ($button >= MAX_BUTTONS_SHOWN-1) {
			break;
		}
	}
	echo "</tr></tbody></table>";
}

function generate_loaddata_from_db($connection, $fileid) {
	$format_string = "function getVertexCount() { return %d; }
	function getdistance() { return -3.0; }\n
	function loadvertices() { return [%s]; }\n
	function loadnormals() { return [%s]; }\n
	function loadtextcoords() { return  [%s]; }\n
	function loadvertexindices() { return [%s]; }\n
	function loadwidth() { return %d; }
	function loadheight() { return %d; }
	function loadtexture() { return( new Uint8Array([%s]) ); }\n";

	$result = mysqli_query($connection, "select uid, name, sequence_length as vertexCount, ppm_height, ppm_width from model where uid = ".$fileid);
	$row = mysqli_fetch_assoc($result);
	$vertex_count = $row["vertexCount"];
	$width = $row["ppm_width"];
	$height = $row["ppm_height"];

	$verticies = array();
	$result = mysqli_query($connection, "select * from vertex where model_file_id = ".$fileid);
	while ($row = mysqli_fetch_assoc($result)) {
		array_push($verticies, $row["x"], $row["y"], $row["z"]);
	}


	$normals = array();
	$result = mysqli_query($connection, "select * from normal where model_file_id = ".$fileid);
	while ($row = mysqli_fetch_assoc($result)) {
		array_push($normals, $row["x"], $row["y"], $row["z"]);
	}

	$texture_coords = array();
	$result = mysqli_query($connection, "select * from texture where model_file_id = ".$fileid);
	while ($row = mysqli_fetch_assoc($result)) {
		array_push($texture_coords, $row["x"], $row["y"]);
	}

	$vertex_indices = array();
	foreach(range(0, $vertex_count-1) as $x) {
		array_push($vertex_indices, $x);
	}

	$texture = array();
	$result = mysqli_query($connection, "select * from ppm where model_file_id = ".$fileid);
	while ($row = mysqli_fetch_assoc($result)) {
		array_push($texture, $row["r"], $row["g"], $row["b"], $row["a"]);
	}
	
	$outputfile = fopen("loaddata.js", "w+") or die("ERROR: Could not create 'loaddata.js' from database");

	fwrite($outputfile, sprintf($format_string,
		$vertex_count,
		implode(", ", $verticies),
		implode(", ", $normals),
		implode(", ", $texture_coords),
		implode(", ", $vertex_indices),
		$width, $height,
		implode(", ", $texture)));
	fclose($outputfile);
	chmod("loaddata.js",  0x1B6); //Universal read-write permissions
}
// END OF A4 FUNCTIONS
// ========================================================================================






//Moves the nth file to the current working directory
function load_file($file_idx) {
	$name = $_FILES["uploadfiles"]["name"][$file_idx];
	move_uploaded_file($_FILES["uploadfiles"]["tmp_name"][$file_idx], "$name");
	$file_info = pathinfo($name);
	return $file_info["dirname"] . "/" .  $file_info["filename"];
}

//Return the full name of the nth file
function name_file($file_idx) {
	return $_FILES["uploadfiles"]["name"][$file_idx];
}

//Run the executable 'a1' on the specified file
function call_a1_executable($call_signature, &$output, $flags) {
	putenv("PYTHONPATH=:.");
	// $output = null;
	$retval = null;
	$perform_action = exec("./a1 ".$flags." $call_signature", $output, $retval);
	echo "<!-- Called exec: ./a1 ".$flags." $call_signature -->";
	echo "<!-- ./a1 returned code: $retval -->";

	//Only load the javascript files if there is a 'loaddata.js'
	if (file_exists("loaddata.js")) {
		echo "<script src=\"webgl-demo.js\" defer></script>";
		echo "<script src=\"loaddata.js\"></script>";
	}
	return $retval;
}


// DATABASE SETUP
// ========================================================================================
//For CLI debug, use command 'mysql -h dursley.socs.uoguelph.ca -u rclark12 -p1120919'
$username = "rclark12";
$password = "1120919";
$database = "rclark12";
$server = "dursley.socs.uoguelph.ca";
$conn = NULL;

establish_db_connection($server, $username, $password, $database, $conn);
if (isset($_POST["dropTables"])) {
	db_drop_all_tables($conn);
}
db_create_all_tables($conn);
// ========================================================================================

echo <<<_HEAD
<!doctype html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Robert's Model Viewer</title>
		<link rel="stylesheet" href="./webgl.css" type="text/css">
		<script src="https://cdnjs.cloudflare.com/ajax/libs/gl-matrix/2.8.1/gl-matrix-min.js" integrity="sha512-zhHQR0/H5SEBL3Wn6yYSaTTZej12z0hVZKOv3TwCUXT1z5qeqGcXJLLrbERYRScEDDpYIJhPC1fk31gqR783iQ==" crossorigin="anonymous" defer></script>

_HEAD;

$numfiles = 0;
$serv_error = 0;
$serv_error_msg = "";

//User has clicked the "Exit" button
if (isset($_POST["haltApp"])) {
	exit("Thank you for using my 3D Model Viewer!");
}

if (isset($_POST["deleteFileNum"]) && !isset($_POST["noDeletion"])) {
	$fileid = $_POST["deleteFileNum"];
	$queries = array();
	array_push($queries, "delete from model where uid = ".$fileid);
	array_push($queries, "delete from vertex where model_file_id = ".$fileid);
	array_push($queries, "delete from normal where model_file_id = ".$fileid);
	array_push($queries, "delete from texture where model_file_id = ".$fileid);
	array_push($queries, "delete from ppm where model_file_id = ".$fileid);
	db_execute_queries($conn, $queries, ECHO_DEBUG);
	if (file_exists("loaddata.js")) {
		unlink("loaddata.js");
	}
}

if (isset($_POST["displayFromDB"])) {
	generate_loaddata_from_db($conn, $_POST["displayFromDB"]);
}

//Only load the javascript files if there is a 'loaddata.js'
if (file_exists("loaddata.js")) {
	echo "<script src=\"webgl-demo.js\" defer></script>";
	echo "<script src=\"loaddata.js\"></script>";
}

//User has selected a model to view, generate the required loader functions
if ($_FILES) {
	$filenames = $_FILES["uploadfiles"]["name"];
	$numfiles = count($filenames); 

	echo "<!-- Num uploaded files:  $numfiles -->";
	
	//User submitted without selecting any files
	if (empty($filenames[0]) && !file_exists("loaddata.js")) {
		$serv_error = 3;
		$serv_error_msg = "Please submit .obj and .ppm files to view...";
	}

	//Database insertion
	else if (isset($_POST["sendToDB"]) && !isset($_POST["dropTables"]) && $numfiles >= 1) {
		foreach(range(0, $numfiles-1) as $x) {
			$output = null;
			$retval = call_a1_executable(load_file($x), $output, "--json");
			if ($retval != 0) {
				$serv_error = 2;
				$serv_error_msg .= "Could not successfully extract information from '".name_file($x)."' <br>";
			}
			$jstr = implode("\n", $output);

			db_insert_JSON_data($conn, $jstr, pathinfo($_FILES["uploadfiles"]["name"][$x])["filename"]);
		}
	}

	//Viewer
	// else if (!isset($_POST["sendToDB"])) {
	else {
		if ($numfiles >= 3) {
			$serv_error = 4;
			$serv_error_msg = "Warning: More than 2 files submitted, attempting to render '".name_file(0)."' and '".name_file(1)."'.";
		}
		if ($numfiles >= 2 && strcmp(load_file(0), load_file(1)) !== 0) {
			$serv_error = 1;
			$serv_error_msg = "Mismatched file names: Cannot render '".name_file(0)."' and '".name_file(1)."', please try again.";
		}
		else if ($numfiles >= 1 && $_FILES["uploadfiles"]["name"][0]!=="") {
			$f1_call_signature = load_file(0);
			$output = null;
			$retval = call_a1_executable($f1_call_signature, $output, "");
			if ($retval != 0) {
				$serv_error = 2;
				$serv_error_msg = "Could not successfully render '".name_file(0)."', please submit matching .obj and .ppm files";
			}
		}
	}
}

echo <<<_BODY
	</head>

	<body>
		<div>
			<form method="post" action="index.php" enctype="multipart/form-data">
				Select Multiple Files to View: <input type="file" name="uploadfiles[]" id="uploadfilename" multiple="multiple">
				<input type="submit" value="Preview">
				<input type="submit" value="Exit" name="haltApp">
			</form>
			<br/>
			<form method="post" action="index.php" enctype="multipart/form-data" style="display: inline-block;">
				Transfer to Database: <input type="file" name="uploadfiles[]" id="uploadfilename" multiple="multiple">
				<input type="submit" value="Submit">
				<input type="submit" value="Delete Tables" name="dropTables">
				<input type="hidden" name="sendToDB">
			</form>
			<form method="post" action="dbdeletion.php" enctype="multipart/form-data" style="display: inline-block;">
				<input type="submit" value="Delete Files">
			</form>
		</div>
		<br>
		<canvas id="glcanvas" width="640" height="480"></canvas><br>
_BODY;

db_generate_display_buttons($conn);

//If there was an error while processing input, inform the user
if ($_FILES) {
	if ($serv_error !== 0) {
		echo "<p style=\"color: red;\">$serv_error_msg</p>";
	}
}

echo <<<_FOOT
	</body>
<html>
_FOOT;

?>