##### 
# 
#
# Execute those steps separately!
#
#
#####
# Step 1
# Get data from ICM
# rsync -r [your_login]@login.icm.edu.pl:/lu/topola/temp/bazinska/results_240/data.zip data.zip
#
#
# Step 2
# Unzip the data
# unzip data.zip >/dev/null
#
#
# Step 3
# Run data_processor with those arguments to concat raw
# data from directories 11 to 22 (excluding)
# from unzipped data.zip:
# python3 data_processor.py --protocol_path=data/ --concat_raw_data=True --from_file=11 --to_file=22 
#
#
# Step 4
# Run data processor on already concatenated
# raw data to sanitize, normalize and split raw data into train/eval/infer sets
python3 data_processor.py --concat_raw_data=False