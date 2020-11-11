#!/bin/bash

#
# update_weather.sh
#
# Skript zum laden von Wetterdaten für Skin FlatPluus
# Daten werden von openweathermap.org geladen. Dazu wird ein API-Key benötigt,
# der kostenlos bezogen werden kann (https://openweathermap.org/price)
#
# Wettersymbole von harmattan: https://github.com/zagortenay333/Harmattan
# Das Skript verwendet 'jq' zum verarbeiten der .json-Dateien
#
# Einstellungen zum Skript in der dazugehörigen *.conf vornehmen!
#
#VERSION=201111

### Variablen ###
SELF="$(readlink /proc/$$/fd/255)" || SELF="$0"  # Eigener Pfad (besseres $0)
#SELF_DIR="${SELF%/*}"                            # Pfad
SELF_NAME="${SELF##*/}"                          # skript.sh
CONF="${SELF%.*}.conf"                           # Konfiguration
DATA_DIR='/tmp/skinflatplus/widgets/weather'     # Verzeichnis für die Daten
# Wetterdaten werden in $DATA_DIR gespeichert
WEATHER_JSON="${DATA_DIR}/weather.json"          # Aktuelles Wetter
LC_NUMERIC=C

### Funktionen ###
f_log(){
  echo "$*"
  logger -t "$SELF_NAME" "$*"
}

f_write_temp(){  # Temperaturwert aufbereiten und schreiben (# $1 Temperatur, $2 Ausgabedatei)
  local data="$1" file="$2"
  printf -v data '%.1f' "$data"  # Temperatur mit einer Nachkommastelle
  printf '%s' "${data/./,}${DEGREE_SIGN}" > "$file"  # Daten schreiben (13,1°C)
}

f_get_weather(){
  local jqdata

  # Wetterdaten laden (One Call API)
  curl --silent --retry 3 --retry-delay 10 --output "$WEATHER_JSON" \
    "https://api.openweathermap.org/data/2.5/onecall?lat=${LATITUDE}&lon=${LONGITUDE}&exclude=minutely,hourly,alerts&units=${UNITS}&lang=${L}&appid=${API_KEY}"

  # Aktuelle Wetterdaten
  printf '%s\n' "$LOCATION" > "${DATA_DIR}/weather.location"                           # Der Ort für die Werte z. B. Berlin
  jqdata=$(jq -r .current.temp "$WEATHER_JSON")
  f_write_temp "$jqdata" "${DATA_DIR}/weather.0.temp"                                  # Aktuelle Temperatur
  jq -r .current.weather[0].description "$WEATHER_JSON" > "${DATA_DIR}/weather.0.description"  # Beschreibung

  # x-Tage Vorhersage
  while [[ ${cnt:=0} -lt $FORECAST_DAYS ]] ; do
    jqdata=$(jq -r .daily[${cnt}].temp.night "$WEATHER_JSON")
    f_write_temp "$jqdata" "${DATA_DIR}/weather.${cnt}.tempMin"                        # Temperatur (Min./Nacht)
    jqdata=$(jq -r .daily[${cnt}].temp.day "$WEATHER_JSON")
    f_write_temp "$jqdata" "${DATA_DIR}/weather.${cnt}.tempMax"                        # Temperatur (Max./Tag)
    jq -r .daily[${cnt}].pop "$WEATHER_JSON" > "${DATA_DIR}/weather.${cnt}.precipitation"  # Niederschlagswahrscheinlichkeit
    jq -r .daily[${cnt}].weather[0].description "$WEATHER_JSON" > "${DATA_DIR}/weather.${cnt}.summary"  # Beschreibung
    jq -r .daily[${cnt}].weather[0].id "$WEATHER_JSON" > "${DATA_DIR}/weather.${cnt}.icon"  # Wettersymbol
    ((cnt++))
  done
}

### Start ###
# Datenverzeichnis erstellen
[[ ! -d "$DATA_DIR" ]] && { mkdir --parents "$DATA_DIR" || exit 1 ;}

# Alte Daten löschen
rm "${DATA_DIR}/weather.*" &>/dev/null

# Konfiguration laden und prüfen
[[ -e "$CONF" ]] && { . "$CONF" || exit 1 ;}
for var in API_KEY FORECAST_DAYS L LOCATION UNITS ; do
  [[ -z "${!var}" ]] && { f_log "Error: $var is not set!" ; RC=1 ;}
done

[[ -n "$RC" ]] && exit 1

# Temp Einheit (standard [°K], metric [°C], imperial [°F])
case $UNITS in
  metric)  DEGREE_SIGN='°C' ;;
  imperal) DEGREE_SIGN='°F' ;;
  *)       DEGREE_SIGN='°K' ;;
esac

f_get_weather  # Wetterdaten holen und aufbereiten

exit
