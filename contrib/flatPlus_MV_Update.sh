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

# In Skriptordner wechseln
cd $(dirname $0)  # Skript im contrib-Ordner

if [ ! -d "../$ICONS" -o ! -d "../$THEMES" -o ! -d "../$DECORS" ] ; then
  echo "Falsches Verzeichnis! Skript muss im ./contrib-Ordner ausgeführt werden"
  exit 1
fi

# Löschen!
echo "-------------------------------"
echo "MV_Themes löschen? (J/n)"
timedout_read 5 TASTE
if [ "$TASTE" = "n" -o "$TASTE" = "N" ] ; then
  echo "Skript abgebrochen. Es wurde nichts gelöscht!"
  exit
fi

rm -rf ../$ICONS/MV*
rm -f ../$THEMES/flatPlus-MV*
rm -f ../$DECORS/*MV*
rm -f ../$INFO
rm -f ../$HIST
echo "MV-Themen wurden entfernt."

# Themen neu laden
echo "-------------------------------"
echo "MV_Themes neu herunterladen und entpacken? (J/n)"
timedout_read 5 TASTE
[ "$TASTE" = "n" -o "$TASTE" = "N" ] && exit

# Download
cd ..
wget https://dl.dropboxusercontent.com/u/1490505/VDR/skinflatplus/MV_Themes.tar.xz

# Entpacken
tar -xJf MV_Themes.tar.xz

# Archiv entfernen
rm -rf MV_Themes.tar.xz

echo "MV-Themen wurden aktualisiert."

exit
