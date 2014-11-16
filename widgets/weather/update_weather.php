<?php

$ini_array = parse_ini_file("./update_weather.config");

$latitude  = $ini_array['Latitude'];
$longitude = $ini_array['Longitude'];
$locationSkin = $ini_array['LocationSkin'];
$units = $ini_array['Units'];
$degree_sign = $ini_array['DegreeSign'];
$lang = $ini_array['Lang'];
$api_key = $ini_array['ApiKey'];

include('lib/forecast.io.php');

// delete old files
array_map('unlink', glob("weather.*"));

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
