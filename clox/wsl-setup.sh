sudo apt-get update
sudo apt-get install dos2unix
sudo apt-get install check
sudo apt-get install pkg-config
sudo apt-get install lcov
echo "Below you should see the flags for check library"
pkg-config --cflags --libs check