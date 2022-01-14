<?php


echo <<<_HEAD
<!doctype html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Robert's Model Viewer</title>
	</head>
_HEAD;
echo <<<_BODY
	<body>
		<div>
			Enter file ID to delete:
			<form method="post" action="index.php" enctype="multipart/form-data" style="display: inline-block;">
				<input type="text" name="deleteFileNum" value="">
				<input type="submit" value="Delete File">
				<input type="submit" value="Return to Model Viewer" name="noDeletion">
			</form>
		</div>
_BODY;

echo <<<_FOOT
	</body>
<html>
_FOOT;

?>