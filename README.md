We give smart functionalities to dumb laundry machines. The four files in the repo are used to detect whether a laundry machine is running or not and to infer when a person has removed their clothes from the machine. The Arduino files also update a MongoDB hosted in Google Cloud with the appropriate information.

Functionalities of the 4 files:

laundry_read_current: Detect when a laundry machine is running or not running. Runs on Arduino Uno

laundry_project_esp_final: Detect from incoming signal from Arduino Uno whether laundry machine is running or not running and update the MongoDB accordingly. Runs on Huzzah Feather

laundry_door_open: Detect when a laundry machine's door is open or closed. Runs on Arduino Uno

esp_update_laundry_door: Detect from incoming signal from Arduino Uno whether or not a person's clothes is in the laundry machine or not. Update the MongoDB accordingly. Runs on Huzzah Feather
