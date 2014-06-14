#!/bin/bash

# flatPlus_MV_Update.sh
# Skript zum Updaten der MV-Themen

#set -x

# Ordner und Dateien
ICONS="icons" ; THEMES="themes" ; DECORS="decors"
INFO=MV_Themes.INFO ; HIST=MV_Themes.HISTORY

timedout_read() {
  timeout=$1 ; varname=$2 ; old_tty_settings=`stty -g`
  stty -icanon min 0 time ${timeout}0
  read $varname
  stty "$old_tty_settings"           # See man page for "stty."
}

### Start
# In Skriptordner wechseln (contrib)
cd $(dirname $0)  # Skript im contrib-Ordner

if [ ! -d "../$ICONS" -o ! -d "../$THEMES" -o ! -d "../$DECORS" ] ; then
  echo "Falsches Verzeichnis! Skript muss im ./contrib-Ordner ausgeführt werden"
  exit 1
fi

if [ -n "$1" ] ; then         # Parameter wurde übergeben
  if [ "$1" = "-u" -o "$1" = "-U" ] ; then
    SILENTUPDATE=1 ; echo "Silent Update! ($1)"
  else
    echo "-------------------------------"
    echo "Falscher Parameter: $1"
    echo "Aufruf mit $(basename $0)"
    echo "Parameter -u    Keine Abfragen (Silent Update)."
    exit
  fi
fi

# MV_Themes Löschen!
if [ -z "$SILENTUPDATE" ] ; then
  echo "-------------------------------"
  echo "MV_Themen löschen? (J/n)"
  timedout_read 5 TASTE
  if [ "$TASTE" = "n" -o "$TASTE" = "N" ] ; then
    echo "Skript abgebrochen. Es wurde nichts gelöscht!"
    exit
  fi
fi

rm -rf ../$ICONS/MV*
rm -f ../$THEMES/flatPlus-MV*
rm -f ../$DECORS/*MV*
rm -f ../$INFO
rm -f ../$HIST
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
wget https://dl.dropboxusercontent.com/u/1490505/VDR/skinflatplus/MV_Themes.tar.xz
tar -xJf MV_Themes.tar.xz    # Entpacken
rm -rf MV_Themes.tar.xz      # Archiv entfernen

echo "-------------------------------"
echo "MV-Themen wurden aktualisiert."

if [ -z "$SILENTUPDATE" ] ; then
  echo "-------------------------------"
  echo "MV_Themen installieren (make install)? (j/N)"
  timedout_read 5 TASTE
  if [ "$TASTE" = "j" -o "$TASTE" = "J" ] ; then
    cd ..
    make install
    echo "-------------------------------"
    echo "MV-Themen wurden installiert."
  fi
else     # Silentupdate - Themen instalieren
  make install
fi

echo "-------------------------------"

exit
