# Matching Simulated Genetic Data with Suffix Trees

## Compiling the code
### In VScode
Open this project in VSCode.  Hit `CTRL+SHIFT+B` (Linux) or `CMD+SHIFT+B` (Mac). To compile the code.

### In the linux terminal
Starting from the top directory of this project (where this README is saved).
```
cd build
cmake ..
make
```

## Running tests
From the `build` directory, you can run
```
./run_tests
```

## Running app
### With pre-built test data
This app reads test data (simulated genomes and genes with mutation) from a file and then determines the accuracy rate of matching the gene based on the single longest substring.  To run the app using pre-built test data from the `build` directory:
```
./run_app your-file-name-here
```
For example:
```
./run_app ../data/genes_5_100000_5000_100_0.1.csv
```
All of the files in the `data` directory with the `.csv` suffix are valid and should be readable by the program.  They contain test data with different parameters, which are encoded in the file names.  The `.txt` files are referenced in the `.csv` file and cannot be passed to the main app.

### Generating new test data
Make sure that you have all dependencies installed for generating test data.   Navigate into the `notebooks` directory.  From `build`, run
```
cd ../notebooks
```
To start a Jupyter Notebook server run
```
jupyter notebook
```

Click on `Generate Genomes.ipynb` in the lefthand sidebar and the notebook should open.  To set parameters for the test data, visit the `Set Parameters` section and modify parameters as desired.

When you are ready, navigate to the `Kernel` menu and select "Restart Kernel and Run All Cells".  At the bottom of the `Simulate Genes` section, the notebook will print a message saying `Saved to test data to your-new-file-path.csv`.  Once this message has been printed, you can copy the new file path and then run the app to see the accuracy rate.

## Dependencies
You will need **gcc** and **cmake** installed to be able to compile this code.  To run the Jupyter Notebook to generate new test data, you will additionally need to have iPython, numpy and pandas.

However, the data structures-related components of this project are all contained in the c++ app and multiple sample datasets are provided.