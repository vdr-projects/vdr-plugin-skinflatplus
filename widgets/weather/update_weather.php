<?php

/*
 * USER CONFIG
 */
$city = "Berlin";
$country = "DE";
$locationSkin = "Berlin"; // location shown in skin
$units = 'si';  // Can be set to 'us', 'si', 'ca', 'uk' or 'auto' (see forecast.io API); default is auto
$degree_sign = "°C";
$lang = 'de'; // Can be set to 'en', 'de', 'pl', 'es', 'fr', 'it', 'tet' or 'x-pig-latin' (see forecast.io API); default is 'en'
// We have only 1000 api calls per day, so please only do one update per day!
// Or request an own key for free at forecast.io
$api_key = '137f2d85a1f1db5762e5e073103541d2';

/*
 * DO NOT CHANGE ANYTHING FROM HERE
 */
include('lib/forecast.io.php');

// delete old files
array_map('unlink', glob("weather.*"));

// get lat & long from google maps
$MAPSURL = "http://maps.googleapis.com/maps/api/geocode/json?address=".$city.",".$country."&sensor=false";
$json = file_get_contents($MAPSURL);
$data = json_decode($json, true);

if( !isset($data['results'][0]) ) {
    echo "no latitude and longitude find for: ".$city.",".$country." !\n";
    exit;
}
$latitude  = $data['results'][0]['geometry']['location']['lat'];
$longitude = $data['results'][0]['geometry']['location']['lng'];

// forecast query
$forecast = new ForecastIO($api_key);

// get daily conditions for next 7 days
$conditions_week = $forecast->getForecastWeek($latitude, $longitude, $units, $lang);

if( !$handle = fopen("weather.location", "w") ) {
    print "can't create file!\n";
} else {
    fwrite($handle, $locationSkin);
    fclose($handle);
}


$index = -1;
foreach($conditions_week as $conditions) {
    $index++;

    if( !$handle = fopen("weather.".$index.".summary", "w") ) {
        print "can't create file!\n";
        continue;
    }
    fwrite($handle, $conditions->getSummary());
    fclose($handle);

/*    
    if( !$handle = fopen("weather.".$index.".temp", "w") ) {
        print "can't create file!\n";
        continue;
    }
    // we only have min & max so we must calc
    $temp = round(($conditions->getMinTemperature() + $conditions->getMaxTemperature()) / 2.0, 1);
    fwrite($handle, $temp);
    fwrite($handle, $degree_sign);
    fclose($handle);
 */
    if( !$handle = fopen("weather.".$index.".tempMin", "w") ) {
        print "can't create file!\n";
        continue;
    }
    // we only have min & max so we must calc
    $temp = round($conditions->getMinTemperature(), 1);
    fwrite($handle, $temp);
    fwrite($handle, $degree_sign);
    fclose($handle);

    if( !$handle = fopen("weather.".$index.".tempMax", "w") ) {
        print "can't create file!\n";
        continue;
    }
    // we only have min & max so we must calc
    $temp = round($conditions->getMaxTemperature(), 1);
    fwrite($handle, $temp);
    fwrite($handle, $degree_sign);
    fclose($handle);


    if( !$handle = fopen("weather.".$index.".precipitation", "w") ) {
        print "can't create file!\n";
        continue;
    }
    fwrite($handle, $conditions->getPrecipitationProbability());
    fclose($handle);

    if( !$handle = fopen("weather.".$index.".precipitationType", "w") ) {
        print "can't create file!\n";
        continue;
    }
    if( $conditions->getPrecipitationProbability() > 0 )
        fwrite($handle, $conditions->getPrecipitationType());
    else
        fwrite($handle, "none");
    fclose($handle);

    if( !$handle = fopen("weather.".$index.".icon", "w") ) {
        print "can't create file!\n";
        continue;
    }
    fwrite($handle, $conditions->getIcon());
    fclose($handle);
}
?>
