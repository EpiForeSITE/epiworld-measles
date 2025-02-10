update-epiworld:
	@echo "Updating epiworld..."
	rsync -avz ../epiworld/epiworld.hpp models/

.PHONY: update-epiworld