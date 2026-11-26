# my-first-IA

A small neural network library written in C++ from scratch, with no machine learning framework: my own vector and
matrix classes, fully connected layers, backpropagation, mini-batch training spread over a thread pool, and a
loader that turns a photo of a handwritten digit into a 28x28 input. I trained it first on XOR, then on MNIST.

The only external code is Sean Barrett's stb_image and stb_image_write headers, used to read and write images.

The identifiers and comments are in French (`Matrice` = matrix, `Couche` = layer, `ReseauDeNeurones` = neural
network, `propagerAvant` = forward pass).

## How it works

### Linear algebra (`Vecteur`, `Matrice`)

`Vecteur` wraps a `std::vector<double>`. `Matrice` stores its values flat in a single `std::vector<double>`, row
by row, and is accessed with `m(i, j)`. They provide what the network needs and nothing more: matrix-vector product,
transpose, element-wise addition and subtraction, scaling, the Hadamard (element-wise) product and the outer
product of two vectors.

### Layers and activations (`Couche`, `Activations`)

A `Couche` is a fully connected layer with a weight matrix `W` (outputs x inputs) and a bias vector `b`, both
initialised with uniform random values in [-1, 1]. The forward pass computes `z = W x + b` and returns
`sigmoid(z)`. It also keeps `z`, which the backward pass needs.

`Activations` implements sigmoid and ReLU with their derivatives, for single values and for whole vectors. Every
layer currently uses sigmoid; ReLU is written but not wired in.

### Loss (`Loss`)

Mean squared error between the output and the target, and its derivative `2/n * (prediction - target)`.

### Backpropagation

For one example, `ReseauDeNeurones::calculerGradientsExemple` runs the forward pass layer by layer and stores each
layer's `z` and activation. It then walks back from the last layer:

- output layer: `delta = dMSE/da ⊙ sigmoid'(z)`
- hidden layer: `delta = (W_nextᵀ · delta_next) ⊙ sigmoid'(z)`
- for every layer: `gradW = delta ⊗ input` (outer product) and `gradb = delta`

This function does not modify the weights, so it can run on several examples at the same time.
`entrainerSurBatch` computes the gradients of every example in the batch, averages them, then applies one gradient
descent step to each layer: `W = W - learning_rate * gradW`, same for `b`.

### Thread pool (`ThreadPool.h`)

A fixed set of worker threads (by default `std::thread::hardware_concurrency()`) waits on a task queue protected by
a mutex and a condition variable. `ajouterTache` pushes a `std::function` and increments a counter of pending
tasks; `attendreFin` blocks until that counter goes back to zero. During training each example of a batch is one
task. The tasks only read the shared weights and each one writes its gradients to its own slot of a result vector,
so they need no locking between them.

### Data loading (`Donnees`)

- `chargerCSV(path, n)`: generic CSV reader, the first `n` columns are the input and the rest is the target (used
  for `xor_data.csv`).
- `chargerMNIST(path)`: reads rows of 785 values (label, then 784 pixels), scales pixels from 0-255 to 0-1 and
  encodes the label as a one-hot vector of size 10. Rows with another length are skipped, so a header line is
  ignored.

### Saving and loading weights

`sauvegarder` writes a plain text file: the number of layers, then for each layer its size, the values of `W` and
the values of `b`. `charger` rebuilds the network from such a file.

### Reading a photo of a digit (`ChargeurImage`)

`chargerImageChiffre` loads any image in grayscale with stb_image and tries to make it look like an MNIST sample:

1. threshold halfway between the darkest and the lightest pixel;
2. bounding box of the dark pixels, plus a 10% margin;
3. resize so the longest side is 20 pixels, keeping the aspect ratio, and centre it in a 28x28 frame (as in MNIST);
4. binarise: ink becomes 1.0, background 0.0.

`afficherASCII` prints the resulting 28x28 input in the terminal, which helps to see what the network receives.

## Programs

| File | What it does |
|---|---|
| `main.cpp` | Trains a 784-128-10 network on MNIST (60 epochs, batch size 32, learning rate 1.0), prints the test accuracy every 5 epochs, then the final train and test accuracy, and saves the weights to `poids_mnist.txt`. |
| `predire.cpp` | Loads `poids_mnist.txt`, reads the image given on the command line, shows it in ASCII and prints the network output for each digit and the predicted digit. |
| `csvVersPng.cpp` | Debugging tool: extracts one row of an MNIST CSV file to a 28x28 PNG with inverted colours (dark ink on a light background, like a photo of paper), to test `predire` on a clean image. |
| `test_cuda.cu` | My first CUDA program: a kernel that prints a message from the GPU. It is not connected to the network yet. |

The XOR experiment lives in the history: an earlier `main.cpp` (commit `d103570`) trained a 2-4-1 network on
`xor_data.csv` for 10,000 epochs with a learning rate of 0.5, saved it to `poids_xor.txt` and reloaded it into a new
network to check that saving works. The training function it used was later replaced by the batch version, so that
`main.cpp` no longer compiles against the current code.

## Building

Any C++17 compiler should work. I use g++ from MSYS2 (UCRT64) on Windows. From the project folder:

```sh
# MNIST training
g++ -std=c++17 -O2 -Wall main.cpp Matrice.cpp Vecteur.cpp Activations.cpp Loss.cpp Couche.cpp Donnees.cpp ReseauDeNeurones.cpp -pthread -o train

# Prediction on an image
g++ -std=c++17 -O2 -Wall predire.cpp ChargeurImage.cpp Matrice.cpp Vecteur.cpp Activations.cpp Loss.cpp Couche.cpp Donnees.cpp ReseauDeNeurones.cpp -pthread -o predire

# MNIST row to PNG
g++ -std=c++17 -Wall csvVersPng.cpp -o csvVersPng

# CUDA test (needs the CUDA toolkit and an NVIDIA GPU)
nvcc test_cuda.cu -o test_cuda
```

The VS Code task in `.vscode/tasks.json` compiles only the file that is open, so it works for `csvVersPng.cpp` but
not for the programs made of several files.

## Getting MNIST

The CSV files are not in this repository. The programs expect the CSV version of MNIST: one image per line, the
label first, then the 784 pixel values (0-255).

- [MNIST in CSV](https://pjreddie.com/projects/mnist-in-csv/) by Joseph Redmon: `mnist_train.csv` and
  `mnist_test.csv`
- or the same data on Kaggle: [oddrationale/mnist-in-csv](https://www.kaggle.com/datasets/oddrationale/mnist-in-csv)
  (these files start with a header line, which the loader skips)

The original dataset is by Yann LeCun, Corinna Cortes and Christopher Burges.

`main.cpp` reads `mnist_medium.csv` (the first 10,000 training images) and `mnist_test_medium.csv` (the first 2,000
test images). To create them from the full files:

```sh
head -n 10000 mnist_train.csv > mnist_medium.csv
head -n 2000 mnist_test.csv > mnist_test_medium.csv
```

With the Kaggle files, skip the header first (`tail -n +2 mnist_train.csv | head -n 10000 > mnist_medium.csv`).

## Usage

```sh
./train                     # trains, prints the accuracy, writes poids_mnist.txt
./csvVersPng mnist_test_medium.csv 0 digit.png   # row 0 of the test set to a PNG
./predire digit.png         # prediction with the saved weights
```

`train` and `predire` read and write their files in the current folder.

## Known limitations

- The output layer uses sigmoid with a mean squared error loss, not softmax with cross-entropy. The ten outputs that
  `predire` prints as percentages are independent sigmoid values, so they do not add up to 100%.
- Weights are initialised uniformly in [-1, 1] without scaling by the layer size.
- Training goes through the examples in the same order at every epoch (no shuffling).
- Matrices are passed and returned by value, so there are many copies. It is easy to read, not fast.
- Recognising digits photographed on paper still often fails: the preprocessing does not yet match MNIST closely
  enough.
- The CUDA part is only a first test, nothing runs on the GPU yet.

## Third-party code

- [stb_image.h](https://github.com/nothings/stb) (v2.30) and stb_image_write.h (v1.16) by Sean Barrett, released
  under your choice of public domain or the MIT license. The license text is at the end of each header.

## Files

```
Vecteur.*, Matrice.*     linear algebra
Activations.*, Loss.*    sigmoid, ReLU, mean squared error
Couche.*                 fully connected layer, forward pass and gradients
ReseauDeNeurones.*       network, batch training, accuracy, save and load
ThreadPool.h             thread pool used for batch training
Donnees.*                CSV and MNIST loading, one-hot encoding
ChargeurImage.*          photo of a digit to a 28x28 input
main.cpp                 MNIST training
predire.cpp              prediction on an image
csvVersPng.cpp           MNIST row to PNG
test_cuda.cu             first CUDA test
xor_data.csv             XOR truth table
poids_xor.txt            weights saved by the XOR experiment
test*.png                small 28x28 test images for predire
stb_image.h, stb_image_write.h   third-party image headers
```

## Context

Personal learning project, started in August 2026, to understand what happens inside a neural network by writing
every part myself. The commit history follows the steps one by one, from linear algebra to the thread pool.

## Author

Antoine Blondeau, embedded systems engineering student at ENSTA.
