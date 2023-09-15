# Language: SED script
# Author: Chimara Okeke
# Code Description: This program uses simple regex-matching properties to detect whether an inputted phone number matches a specific format.
# If yes, the program outputs a randomized string of letters as a form of encrypting the inputter number.
# To run: This file is called "phone_encrypter.sed". You shall need another file of phone numbers called "phone.txt".
# In a terminal, such as Putty, type: sed -r -f phone_encrypter.sed phone.txt 
# The output is a list of the encrypted phone numbers.

s/[(|)]//
s/[)]//
s/\s//
s/-//
s/^(..)(.)(....)(.)(..)/\1\4\3\2\5/
s/[5]/x/g
s/[2]/P/g
s/[8]/m/g
s/1/O/g
s/0/W/g
s/9/s/g
s/4/i/g
s/3/q/g
s/6/L/g
s/7/u/g
3 a\ChimaraOke
