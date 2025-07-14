CC = @echo "🏎️  𖦹 🏁 Compiling Rally - Pannel" && gcc
DEP = src/main.c src/bargraph.c src/data.c src/shiftlights.c src/utils.c src/dashboard.c
OUTPUT = output/build/app
LAUNCH = @echo "Lauch Rally - Pannel" && ./output/build/app

build: $(DEP)
	@echo "MK - build" && mkdir -p output/build
	$(CC) $(DEP) -o $(OUTPUT) -lncurses
	$(LAUNCH)