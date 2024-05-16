#!/bin/bash

# Shell Script for Palindrome Check

# Function to check if a given string is a palindrome
is_palindrome() {
  original=$1
  reversed=$(echo $original | rev)

  if [ "$original" == "$reversed" ]; then
    echo "The string '$original' is a palindrome."
  else
    echo "The string '$original' is not a palindrome."
  fi
}

# Function to reverse a string
reverse_string() {
  original=$1
  reversed=$(echo $original | rev)
  echo "Original String: $original"
  echo "Reversed String: $reversed"
}

# Function to count the number of characters in a string
count_characters() {
  original=$1
  length=${#original}
  echo "Number of characters in '$original': $length"
}

# Main Script

echo "Palindrome Check using Shell Script"

# Take user input
read -p "Enter a string: " input_string

# Call function to check if the input string is a palindrome
is_palindrome "$input_string"

# Call function to reverse the input string
reverse_string "$input_string"

# Call function to count the number of characters in the input string
count_characters "$input_string"

# Demonstrate a for loop by printing each character of the string
echo "Printing each character of the string:"
for ((i=0; i<${#input_string}; i++)); do
  echo "${input_string:$i:1}"
done

# Demonstrate a while loop by reversing the string character by character
echo "Reversing the string character by character using a while loop:"
len=${#input_string}
reversed=""
i=$((len-1))
while [ $i -ge 0 ]; do
  reversed="$reversed${input_string:$i:1}"
  ((i--))
done
echo "Reversed String: $reversed"

echo "Script execution complete."
