if [ -f /usr/lib/libclr.so ]; then
  sudo gcc -O3 -o /usr/bin/clr src/main.c src/optionhandler.c src/colourizer.c -lclr
else
  echo libclr not found
  echo install from https://www.github.com/velox0/libclr
fi
