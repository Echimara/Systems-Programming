**FOR PROJECT _LINK_ USAGE**
1. Launch two cell machines or terminals (e.g. cse06 and cse05)
2. On one, run minor4svr.c as follows:
    ● gcc -o minor4svr minor4svr.c
    ● ./minor4svr 8001
3. You will see: [server]: ready to accept data... if successful
4. On the other CSE machine, run minor4cli.c as follows:
    ● gcc -o minor4cli minor4cli.c
    ● ./minor4cli <hostname that server program is running on> 8001
5. The program will send 10 PINGs to the server program
6. The program then prints total packets, lost packets, percentage of packets lost, etc
7. Results from running minor4cli.c will vary based on the chosen port number.

**S/N:**  
    “8001” above is one of many registered port numbers. This can be replaced with another port between the
    ranges 1024 to 49151. In the sample output provided, the program does not echo the ‘PONG’ message sent
    back from the server to the client. I included that print statement in my server side of the code.
    Citation: I learned about and drew inspiration for making my UDP programs using this source:
    GeeksforGeeks. All of my work was revised and well understood, the concepts included. My work was also
    completed individually.
