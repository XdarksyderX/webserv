<?php

parse_str(getenv("QUERY_STRING"), $_GET);

$continente = $_GET['continente'];
$n_dias = $_GET['n_dias'];

$priceLocation = 0;

if ($continente == 'america') {
    $priceLocation = 4;
} elseif ($continente == 'europa') {
    $priceLocation = 1;
} elseif ($continente == 'asia') {
    $priceLocation = 3;
} elseif ($continente == 'africa') {
    $priceLocation = 2;
} elseif ($continente == 'oceania') {
    $priceLocation = 5;
} else {
    $priceLocation = 0;
}

$total = intval($n_dias) * $priceLocation;

echo "total = " . strval($total);
?>
