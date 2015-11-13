# Data manager
Download, parse, insert in the database and prepare the data for the neural network.

## Installation & Initialisation
* Build data-manager
* Put the executable wherever you want
* Copy data-manager.conf next to it
* Run init.sh [absolute-path-to-data] [absolute-path-to-conf]

## Usage
### Download
To download, use the following syntax
```
$ ./data-manager download from [date] to [date]
```
The dates have to be formated in a `yyyy-mm-dd way`. You can aslo use the keywords `today` and `yesterday`.
The two dates are set to `today` by default.
