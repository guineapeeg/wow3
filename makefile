all: myChannels
my_userid: my_userid.c
	gcc -g -Wall -o myChannels myChannels.c

clean:
	$(RM) myChannels