CC=cc
CFLAGS=-Wall -g
OUT_DIR=bin
SRC_DIR=src
INCLUDES=include

day:
	@if [ "$(DAY)" ]; then \
	  echo "Compiling Day: $(DAY)"; \
	  if [ ! -d "$(SRC_DIR)/day_$(DAY)" ]; then \
	    echo "Day has not been started yet"; \
	    exit; \
	  fi; \
	  mkdir -p $(OUT_DIR)/day_$(DAY); \
	  for fsrc in $$(ls $(SRC_DIR)/day_$(DAY)/*.c); do \
	    echo "\t.. $$fsrc"; \
	    fname="$$(basename $$fsrc .c)"; \
	    cc -I$(INCLUDES) $(CFLAGS) -o $(OUT_DIR)/day_$(DAY)/$$fname $$fsrc; \
	  done; \
	fi

run:
	@if [ "$(DAY)" ]; then \
	  echo "Solutions for Day: $(DAY)"; \
	  for sol in $$(ls $(OUT_DIR)/day_$(DAY)); do \
	    ./$(OUT_DIR)/day_$(DAY)/$$sol; \
	  done; \
	fi
clean:
	rm -r $(OUT_DIR)
