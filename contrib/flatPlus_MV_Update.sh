#!/bin/bash

# flatPlus_MV_Update.sh
# Skript zum Updaten der MV-Themen

#set -x

# Ordner und Dateien
ICONS="icons" ; THEMES="themes" ; DECORS="decors" ; CONFIGS="configs"
INFO=MV_Themes.INFO ; HIST=MV_Themes.HISTORY

# Ordner für Direktupdate (Im aktuellem Filesystem); Beispiel für Gen2VDR
THEMEDIR="/etc/vdr/themes"
ICONDIR="/etc/vdr/plugins/skinflatplus/icons"
DECORDIR="/etc/vdr/plugins/skinflatplus/decors"
CONFIGSDIR="/etc/vdr/plugins/skinflatplus/configs"

timedout_read() {
  timeout=$1 ; varname=$2 ; old_tty_settings=`stty -g`
  stty -icanon min 0 time ${timeout}0
  read $varname
  stty "$old_tty_settings"           # See man page for "stty."
}

_help() {
  echo "-------------------------------"
  echo "Falscher Parameter: $1"
  echo "Aufruf mit $(basename $0)"
  echo "Parameter -Silent    Keine Abfragen (Silent Update)."
  echo "Parameter -Direct    Direktes Update der Daten im Filesysten (/etc/vdr)."
  echo "                     !!! Speziell für Gen2VDR angepasst !!!"
  exit
}
### Start
# In Skriptordner wechseln (contrib)
cd $(dirname $0)  # Skript im contrib-Ordner

if [ ! -d "../$ICONS" -o ! -d "../$THEMES" -o ! -d "../$DECORS" ] ; then
  echo "Falsches Verzeichnis! Skript muss im ./contrib-Ordner ausgeführt werden"
  exit 1
fi

if [ -n "$1" ] ; then         # Parameter wurde übergeben
  case $1 in
    -Silent) SILENTUPDATE=1 ; echo "Silent Update! ($1)" ;;
    -Direct) DIRECTUPDATE=1 ; echo "Direct Update! ($1)" ; unset SILENTUPDATE ;;
    *)       _help
  esac
fi

# MV_Themes Löschen!
if [ -z "$SILENTUPDATE" ] ; then
  echo "-------------------------------"
  echo "MV_Themen löschen? (J/n)"
  [ -n "$DIRECTUPDATE" ] && echo "ACHTUNG: Dateien in /etc werden gelöscht!"
  timedout_read 5 TASTE
  if [ "$TASTE" = "n" -o "$TASTE" = "N" ] ; then
    echo "Skript abgebrochen. Es wurde nichts gelöscht!"
    exit
  fi
fi

if [ -n "$DIRECTUPDATE" ] ; then # Löschen im Dateisystem (/etc)
  [ -d $ICONDIR ] && rm -rf $ICONDIR/MV*
  [ -d $THEMEDIR ] && rm -f $THEMEDIR/flatPlus-MV*
  [ -d $DECORDIR ] && rm -f $DECORDIR/*MV*
else                             # Löschen im Source-Dir von skinflatPlus
  rm -rf ../$ICONS/MV*
  rm -f ../$THEMES/flatPlus-MV*
  rm -f ../$DECORS/*MV*
  rm -f ../$INFO
  rm -f ../$HIST
fi

echo "MV-Themen wurden entfernt."

# MV_Themes neu laden
if [ -z "$SILENTUPDATE" ] ; then
  echo "-------------------------------"
  echo "MV_Themes neu herunterladen und entpacken? (J/n)"
  timedout_read 5 TASTE
  [ "$TASTE" = "n" -o "$TASTE" = "N" ] && exit
fi

cd ..
# Download
[ -n "$DIRECTUPDATE" ] && cd /tmp
wget https://dl.dropboxusercontent.com/u/1490505/VDR/skinflatplus/MV_Themes.tar.xz
tar -xJf MV_Themes.tar.xz    # Entpacken
rm -f MV_Themes.tar.xz       # Archiv entfernen

if [ -n "$DIRECTUPDATE" ] ; then
  cp -f ${THEMES}/flatPlus-MV* $THEMEDIR
  cp -f ${DECORS}/*MV* $DECORDIR
  cp -rf ${ICONS}/MV* $ICONDIR
  cp -f ${CONFIGS}/*MV* $CONFIGSDIR
fi

echo "-------------------------------"
echo "MV-Themen wurden aktualisiert."

[ -n "$DIRECTUPDATE" ] && exit

if [ -z "$SILENTUPDATE" ] ; then
  echo "-------------------------------"
  echo "MV_Themen installieren (make install)? (j/N)"
  timedout_read 5 TASTE
  if [ "$TASTE" = "j" -o "$TASTE" = "J" ] ; then
    #cd ..
    make install
    echo "-------------------------------"
    echo "MV-Themen wurden installiert."
  fi
else     # Silentupdate - Themen instalieren
  make install
fi

echo "-------------------------------"

exit
