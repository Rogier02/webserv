#!/usr/bin/env php
<?php

$html = file_get_contents(__DIR__ . '/time.html');
echo $html;
// Get the current time
$current_time = date('Y-m-d H:i:s');

// Output the current time
echo "<H1>" . $current_time . "\n</H1>";

?>
