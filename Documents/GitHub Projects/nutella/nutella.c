/* 

CS3516 B09
Project2 nutella
Xiaoli Ma

*/
#include "msock.h"
#include <fcntl.h>


void main() {
   char message[MESSAGE_LEN];
   int len, sock,sock2, cnt,cnt2;
   char buf[MESSAGE_LEN];
   char recv[MESSAGE_LEN];
   char rec[MOVIE_LEN];
   FILE * file;
   FILE * fp;
   // assign ip for different movies
   char ip_star[]="239.0.0.5";  
   char ip_pong[]="239.0.0.6";
   char ip_clerk[]="239.0.0.7";
   char ip_walk[]="239.0.0.8";
   char flags[MESSAGE_LEN];
   
   int flag=0,FLAG=0;
   int selection; //whether client or server
   int flag2=0;
   int i=1,j=1,count=0;
   
   
   
    
   printf("Search for movie? Y/N\n");
   
   
   try:
  while(selection !=89 | selection !=78){  
   
   
   scanf("%s",&selection);
   

   if (selection==89) {    //////////////ask for movie//////////////////////
              
     printf("Please enter a movie name to search...\n");
	 printf("Available moives are: \n\n");
	 printf("Walk\n\n");
	 printf("Pong\n\n");
	 printf("TigerCub\n\n");
	 printf("StarWars\n\n");
	 
	 
    again:

     while (flag2==0) {  //flag is set to 1 when client quits
		if((sock=msockcreate(SEND,IPADDR,PORT))<0){
			perror("Cannot open stream socket");
			exit(1);
		}
		
		printf("What would you like?\n");
		scanf("%s",message);
		printf("Your query is: %s\n",message);
	 
       printf("sending query: %s\n", message);
       cnt = msend(sock, message, strlen(message)+1);
       
       if (cnt < 0) {
		perror("msend");
		msockdestroy(sock);
		exit(1);
       }
       
	   
	   mrecv(sock,buf,sizeof(buf));
		 FLAG = fcntl(sock,F_GETFL,0);
	   FLAG |= O_NONBLOCK;
	   fcntl(sock,F_SETFL,FLAG);
		
	   if(strcmp(buf,"False")==0){   //if server send "False" info, goto again and type in movie name again
	   printf("Fail to find the movie,quit....\n");
	   printf("Please select another!\n");
	   goto again;
	   
	   }else{ 
		mrecv(sock,recv,sizeof(recv));  //recv assigned ip addr if movie is available
		
		
	   
		printf("Given IP_addr and port are : %s\n",recv);
	
		//create the new sock with new IP and PORT
		if((sock2=msockcreate(RECV,recv,PORT))<0){
		perror("Cannot open stream socket");
		msockdestroy(sock2);
		exit(1);
		
		}
			printf("Connecting %s at port 55555",recv);
				printf("\n");
				mrecv(sock2,flags,sizeof(flags));
				count=atoi(flags);
				while(1){	
					
				mrecv(sock2,rec,sizeof(rec));
				  printf("%s",rec);          // print movie as it receives
					
					
				if(strcmp(rec,"stop\n")==0){     //if movie is done, choose another or not
					msockdestroy(sock2);
					while(1){
					printf("Your moive is done!\n");
					printf("Want another one? Y/N\n");
					scanf("%s",buf);
					if(strcmp(buf,"Y")==0){
					flag2=1;
					  break;
					}else if(strcmp(buf,"N")==0){
						printf("Thank you. Quit....\n\n");
						
						exit(1);
					}else {
						printf("Answer by Y/N\n");
						
					}
					continue;
					}
					
					if(flag2==1){
					flag2=0;
					goto again;									//ask for another movie
					}
				}else
				continue;
				
			}
		
	   }
	  
	 sleep(SLEEP_TIME);
	  continue;
     } 
	 
	 ///////////////////////////////Player/////////////////////////////////////
   } else if(selection==78){  
     
    
     wait:	
	 
	 printf(" Waiting for messages...\n");
	 printf("Ctrl-C to close....\n\n");
     
	  /* receiver plays out messages */
     while (1) {
	/* set up socket */
     if ((sock=msockcreate(RECV, IPADDR,PORT)) < 0) {
       perror("msockcreate");
       exit(1);
     }
	 cnt = mrecv(sock, message, MESSAGE_LEN);
	 FLAG = fcntl(sock,F_GETFL,0);
	 FLAG |= O_NONBLOCK;
	 fcntl(sock,F_SETFL,FLAG);
	 
       
       if (cnt < 0) {
	 perror("mrecv");
	 msockdestroy(sock);
	 exit(1);
       } else if (cnt==0) {
	 break;
       }
       printf("Query is %s \n", message);
	   
	   //////////////check if starwars//////////////////
	   if(strcmp(message,"StarWars")==0){       //parse query ___StarWars
		
	    char buf[]="True";
		msend(sock,buf,strlen(buf)+1);         //inform client that movie is found
		//assign corresponding ip/port
	    msend(sock,ip_star,sizeof(ip_star));
		printf("IP_addr assignment is: %s\n",ip_star);
		msockdestroy(sock);
		printf("Connecting.....\n");
		sleep(SLEEP_TIME);
		
		if ((sock2=msockcreate(SEND, ip_star,PORT)) < 0) {   //create another sock for moive streaming
       perror("msockcreate");
       exit(1);
     }
		printf("Stream StarWars!\n");
		file=fopen("starwars.mov","r+");
		if(file!=NULL){
			i=1;
			for(j=1;j<=82;j++){						     //82 frames
			clearscreen();								//clear screan per frame
			for(count=1;count<=8;count++){
				while(fgets(rec,100,file)!=NULL && i<=8){       //8 lines a frame
				i++;
				
				}
			if(strcmp(rec,"stop\n")==0){
			msend(sock2,rec,sizeof(rec));        
			fclose(file);
			goto wait;											//want another movie
			}
			msend(sock2,rec,sizeof(rec));					//send lines to client
			printf("%s",rec);
			}
			usleep(MOVIE_INT);								//sleep 75000 miliseconds per frame
			}
		}else{
			printf("Error, quit...\n");
		//	msockdestroy(sock2);
			exit(1);
		}
		break;
	   }/////////////////////////////end if Star//////////////////////////
	   else if(strcmp(message,"Pong")==0){						//parse moive ___pong
	   
	    char buf[]="True";
		msend(sock,buf,strlen(buf)+1);
		//assign corresponding ip/port
	    msend(sock,ip_pong,sizeof(ip_pong));
		printf("IP_addr assignment is: %s\n",ip_pong);
		
		msockdestroy(sock);
		printf("Connecting.....\n");
		sleep(SLEEP_TIME);
		
		if ((sock2=msockcreate(SEND, ip_pong,PORT)) < 0) {
		perror("msockcreate");
		exit(1);
		}
		printf("Stream Pong!\n");
		file=fopen("pong.mov","r+");
		if(file!=NULL){
			i=1;
			for(j=1;j<=28;j++){										//28 frames
			clearscreen();
				
				for(count=1;count<=10;count++){						//10 lines per frame
					while(fgets(rec,100,file)!=NULL && i<=10){
					i++;
					
					}				
					
					if(strcmp(rec,"stop\n")==0){
					msend(sock2,rec,sizeof(rec));
					fclose(file);
					goto wait;											//want another movie
					}
				msend(sock2,rec,sizeof(rec));							//send movie 
				printf("%s",rec);

					}
				usleep(MOVIE_INT);										//sleep for 75000 miliseconds
			
			
			}
		}else{
			printf("Error, quit...\n");
			//msockdestroy(sock2);
			exit(1);
		}
		
		break;
	   }///////////////////end if pong/////////////////////////////////
	   else if(strcmp(message,"TigerCub")==0 ){								//parse for tigerCub
	   
	   char buf[]="True";
		msend(sock,buf,strlen(buf)+1);
		//assign corresponding ip/port
	    msend(sock,ip_clerk,sizeof(ip_clerk));
		printf("IP_addr assignment is: %s\n",ip_clerk);
		msockdestroy(sock);
		printf("Connecting.....\n");
		sleep(SLEEP_TIME);
		
		if ((sock2=msockcreate(SEND, ip_clerk,PORT)) < 0) {						//new sock for movie
       perror("msockcreate");
       exit(1);
     }
		printf("Stream TigerCub!\n");
		file=fopen("tiger.mov","r+");
		if(file!=NULL){
			i=1;
			for(j=1;j<=18;j++){													//18 frames
			
				clearscreen();
				for(count=1;count<=14;count++){                                 //14 lines per frame
					while(fgets(rec,100,file)!=NULL && i<=14){
					i++;
				}
				
				if(strcmp(rec,"stop\n")==0){
				msend(sock2,rec,sizeof(rec));
			fclose(file);
			goto wait;														//if want another movie
			}
				msend(sock2,rec,sizeof(rec));								//send movie
				printf("%s",rec);
				}
			usleep(MOVIE_INT);											//sleep 75000 miliseconds
			
			}
		}else{
			printf("Error, quit...\n");
		//	msockdestroy(sock2);
			exit(1);
		}
		break;
	   }/////////////////////end if clerk//////////////////////////////
	   
	   else if(strcmp(message,"Walk")==0){								//parse for walk
	    
	    char buf[]="True";
		msend(sock,buf,strlen(buf)+1);
		//assign corresponding ip/port
	    msend(sock,ip_walk,sizeof(ip_walk));
		printf("IP_addr assignment is: %s\n",ip_walk);
		msockdestroy(sock);
		printf("Connecting.....\n");
		sleep(SLEEP_TIME);
		
		if ((sock2=msockcreate(SEND, ip_walk,PORT)) < 0) {						//new sock for movie
       perror("msockcreate");
       exit(1);
     }
	 //send the count to refresh screen
	 
		char flags[]="5";
		msend(sock2,flags,sizeof(flags));
		
		printf("Stream Walk!\n");
		file=fopen("walk.mov","r+");
		if(file!=NULL){
			i=1;
			for(j=1;j<=10;j++){											//10 frames
				clearscreen();																
				
				for(count=1;count<=5;count++){								//5 lines per frame
					while(fgets(rec,100,file)!=NULL && i<=5){
					i++;
					
					}
				
				if(strcmp(rec,"stop\n")==0){
				msend(sock2,rec,sizeof(rec));
					fclose(file);
					
					goto wait;                                              //ask if want another moive
			}
				msend(sock2,rec,sizeof(rec));								//send movie
				printf("%s",rec);
				}
			usleep(MOVIE_INT);
			
			
			}
		}else{															//quit if cannot open file
			printf("Error, quit...\n");
			//msockdestroy(sock2);
			exit(1);
		}
		break;
	   }
	  /////////////////// //end if walk///////////////////
	   else{  													//indicates that query is not available
		printf("Your search is not available.\n");
        char buf[]="False";
		msend(sock,buf,strlen(buf)+1);
		goto wait;
	   }
	   sleep(SLEEP_TIME);
     }
   }else{																	//answer for search movie or not is other than Y or N
   printf("Please response by Y/N\n");
  
   goto try;																//try to answer again
   }//return to init
   }
   
}
