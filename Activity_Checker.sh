#!/bin/bash
#Name: Chimara Okeke
#EUID: ceo0079
#Course: CSCE 3600.002
#Recitation Section: 205
#Due date: 3/3/2023 @ 11:59PM
#This bash script monitors and reports who logs in to and out of whichever
#linux machine the script is ran on. Reports are printed every 10 seconds (if any)
#along with the date and number of logged in users. A custom hangler is present
#to trap the SIGINT or ctrl+C shortcut once before terminating the script.

#Definition of function, custom_handler.
#It traps any first input of ctrl+C i.e. SIGINT
custom_handler()
{
echo "(SIGINT) ignored. enter ^C 1 more time to terminate program."
trap SIGINT
}
#A set-up of the previously defined signal handler
trap custom_handler SIGINT
#"f_date" -> a formatted system date to match the sample output in minor2 prompt
f_date=$(date +"%a %b %d %H:%M:%S %Z %Y")
#"host" -> hostname gets the hostname of current machine
#hostname output is piped into a sed subsitution command

#that replaces "-cse" with nothing (to match minor2 sample output)
#Finally, "host" is initialized with the truncated hostname
host=$(hostname | sed 's/-cse//')
#Prints list of initially logged in users and total user count
echo "$f_date) initial users logged in"
who -u | awk -v host="$host" '{print "> " $1 " logged in to " host}'

#An continuous while loop that is terminated by a second SIGINT
while true; do
# A while loop that reads lines from the redirected output of "last -F"
while read -r line; do
#If a line has an 11th parameter of "in" and a 9th parameter of the current time, that
line's 1st parameter is
#a user who just logged in to the system
if [[ $(echo "$line" | awk '{print $11}') == "in" && $(echo "$line" | awk '{print $9}') == $(date
+"%T") ]]; then
# Prints all users who logged into the machine at the specific time
echo "> $(echo "$line" | awk '{print $1}') logged in to $host"
#Else if a line has a 9th parameter of em-dash and a 13th parameter of the current time,
that line's 1st parameter is
#a user who just logged out of the system
elif [[ $(echo "$line" | awk '{print $9}') == "-" && $(echo "$line" | awk '{print $13}') == $(date
+"%T") ]]; then
# Prints all users who logged out of the machine at the specific time
echo "> $(echo "$line" | awk '{print $1}') logged out of $host"
fi
#Displays the current date and total user count
#The who command output is piped into wc -l command which count all lines in who.
#The value is passed via parameter and displayed via echo as $(who | wc -l)
#The date value has been formatted to match the minor2 sample output
echo "$(date +"%a %b %d %H:%M:%S %Z %Y")) # of users: $(who | wc -l)"
#Allows the loop to repeat every 10 seconds
sleep 10
#Output redirection similar to piping
done < <(last -F)
done
