if [ -f /usr/lib/libclr.so ]; then
  sudo gcc -O3 -o /usr/bin/clr src/main.c src/clusterfudge.c src/colourizer.c src/optionhandler.c -lclr
else
  echo libclr not found
  echo install from https://www.github.com/velox0/libclr
  exit 1
fi

gzip -fk ./clr.6

manpath="/usr/share/man/man6/"
if [ ! -d "$manpath" ]; then
  # Create the path
  sudo mkdir -p "$manpath"
fi

sudo mv ./clr.6.* $manpath
