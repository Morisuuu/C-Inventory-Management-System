/*
CS103 Programming Project

Members:
Ingua, Manuel
Foronda, Yna Gabrielle
Miranda, Francis Maurice

PROJECT TITLE
A virtual grocery system where users have access to items available in
your average supermarket digitally. The project consists of tons of features such as a
login system where the user's information are stored and are given a chance to gain loyalty points
for discounts, a cart system where customers can add and remove items that are presented infront
of them, and an admin system where authorized individuals are able to monitor and change the stocks
of items that are in their market. 
*/

/*header files*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define MAX 200

//creating a self-referential structure for the item linked list
struct item {
    int code; //item code
    char name[31]; //item name (max 30 char)
    float price; //item price
    int stock; //item stock
    struct item *link; //pointer to node
} *headinv=NULL, *tailinv=NULL, *headcart=NULL, *tailcart=NULL; //declared linked list pointers

/* -------------FUNCTION DECLARATIONS-----------------*/

/*declared/initialized universal variables*/
int opt, check;
float totalr=0, total=0, points=0, discount=0;

int user_check = 0;
char username[30], password[30];

FILE *inventory = NULL;
FILE *user_cart = NULL;

/*sign-up/login function declarations*/
void AdminSignUp(FILE * userlist, FILE * passlist);
void Login(FILE * userlist, FILE * passlist);
void SignUp(FILE * userlist, FILE * passlist);
void AdminSignUp(FILE * userlist, FILE * passlist);
void LoginMenu(void);
void helpLogin(void);

/*cart functions declarations*/
void writeCart(void);
struct item* readCart(void);
void compare(void);
void CustomerMenu(void);
void showCart(void);
void Receipt(void);
void AddItems(void);
void RemoveItems(void);
void ApplyPoints(void);
void helpCart(void);

/*inventory functions declarations*/
void adminmenu(void);
void addp(void);
void deletep(void);
void updatep(void);
void showInv(void);
struct item* readinv(void);
void writeinv(void);
void helpAdmin(void);

/*other functions declarations*/
void red(void) {printf("\033[1;31m");} //prints text in red
void reset(void) {printf("\033[0m");} // resets the text color to white
void blue(void) {printf("\033[0;34m");} //prints text in blue
void green(void) {printf("\033[0;32m");} //prints text in green
void delaynext(long milli);

/* reoccuring functions definition/guide 

- system("clear || cls") - clears the screen. "clear" for windows and "cls" for linux
- getc(stdin) - gets any character input as feedback
- rewind() - sets the file position to the beginning of the file of the given stream
- fflush() - clears (or flush) the output buffer and move the buffered data to console
- puts("") - appends a newline

*/

/* --------------------MAIN()------------------------*/

int main() {
    system("clear || cls");
    blue(); printf("\n\tCtrl + G"); reset(); //outputs project title with specified format
    printf("\n\tA virtual grocery system where admins have control\n");
    printf("\thave control over their market's product listing and\n");
    printf("\twhere customers have control over the products they avail\n");
    printf("\t(+ a gift for loyal ones).\n");
    printf("\n\n\n\tPress enter to continue..."); //prompt for user input
    getc(stdin);
    LoginMenu(); //function calls LoginMenu()
    return 0;
}

/* ----------------------------------------------------LOGIN FUNCTIONS--------------------------------------------------------------- */

//function for the login menu
void LoginMenu(void){
    headinv=tailinv=headcart=tailcart=NULL; //sets the head and tail of the linked list for the inventory and cart to NULL
    int back=1;
    //code sequence for login menu
    while(back==1) {
        system("clear || cls");
        blue(); printf("\n\tWELCOME!   \n");
        printf("\t----------------------------------------------------\n");
        reset();
        //outputs the login/sign-up options
        printf("\n\t<1> Log-in\n");
        printf("\t<2> Sign-up\n");
        printf("\t<3> Continue as Guest\n");
        printf("\t    [ You won't get our discounts and save your cart ]\n");
        printf("\t<4> Help\n");
        printf("\t<5> Exit\n\n");

        //loop that gets input from the user
        do {
            printf("\t> Input response: ");
            fflush(stdin);
            check=scanf("%d", &opt);
        } while (check==0); //checks if input is valid

        //declaring file pointers for username list and password list
        FILE * userlist;
        FILE * passlist;

        //switch case for the option chosen by the user
        switch(opt){
            case 1: system("clear || cls"); Login(userlist, passlist); back=0; break; //option 1 function calls Login()
            case 2: system("clear || cls"); SignUp(userlist, passlist); back=0; break; //option 2 function calls SignUp()
            case 3: CustomerMenu(); back=0; break; //option 3 function calls CustomerMenu() - redirect to guest cart
            case 4: system("clear || cls"); helpLogin(); break;
            case 5: printf("\n\t[ Exiting program. ]"); exit(0); //option 4 terminates the program
            case 65: system("clear || cls"); AdminSignUp(userlist, passlist); back=0; break; //case that 65 is inputted function calls AdminSignUp()
            default: printf("\n\t[ Invalid option, please try again. ]"); delaynext(1000); //redirects to the login menu if input is invalid
        }
    }
}

//function to display the help information
void helpLogin(void){
    blue();
    //outputs help information
    printf("\n\tH E L P \n");
    printf("\t----------------------------------------------------\n");
    reset();
    green();
    printf("\t[ Log-In ]\n");
    reset();
    printf("\tIf you have a registered account, you can log-in by\n");
    printf("\ttyping in your username and password. If you don't\n");
    printf("\thave an account yet, proceed to [ Sign-Up ].\n\n");
    green();
    printf("\t[ Sign-Up ]\n");
    reset();
    printf("\tHere you can create an account by filling up the\n");
    printf("\trequired fields. After creating an account, you can\n");
    printf("\tproceed to [ Log-In ].\n\n");
    green();
    printf("\t[ Continue as guest ]\n");
    reset();
    printf("\tYou can continue as guest if you want to shop without\n");
    printf("\tusing an account. Do note that you do not earn points\n");
    printf("\tdoing so and your cart won't be saved.\n\n");
    printf("\tPress enter to go back to the [ Login Menu ].");
    fflush(stdin);
    getc(stdin);
}

//function for Log-In
void Login(FILE * userlist, FILE * passlist){
    char userhold[30], passhold[30]; //variables to get the input
    int count=0, count2, linescount, check = 0;
    char adminname[] = "admin";

    blue();
    printf("\n\tLOG-IN\n");
    printf("\t----------------------------------------------------");
    reset();
    //prompt for username and password
    printf("\n\tUsername: ");
    scanf("%s", username);
    printf("\tPassword: ");
    scanf("%s", password);

    userlist = fopen("username.txt","r"); //opens username.txt as read

    while (!feof(userlist)) { //loop to iterate until all lines has been read
        fscanf(userlist, "%s", userhold); //reads the current line in username.txt
        count ++; //increments line counter
        if(strcmp(userhold, username) == 0){ //checks if userhold and username are the same
            passlist = fopen("password.txt","r"); // opens password.txt as read
            for(count2 = 1 ; count2 <= count ; count2 ++){ //for-loop that goes through the passlist
                fscanf(passlist, "%s", passhold); //reads the current line in password.txt
                if (count2 == count) { //checks if count2 and count1 are the same
                    if(strcmp(passhold,password) == 0){  // checks if passhold and password are the same
                        green(); printf("\t[ You are now logged in! ]"); reset(); //outputs successful login message
                        delaynext(1000);
                        //for-loop to check if the username has the word "admin"
                        for(int i = 0; i < 5; i++){
                            if(username[i] == adminname[i])
                                check++; //increments check on the amount of similar characters
                        }
                    } else{
                        red(); printf("\t[ Invalid password. ]"); reset(); //outputs error message otherwise
                        delaynext(1000);
                        fclose(userlist);
                        fclose(passlist);
                        LoginMenu();
                    }
                }
            }
            fclose(userlist);
            fclose(passlist);
            delaynext(1000);
            if (check == 5){ //if check is equal to 5 or the checked username has "admin"
                adminmenu(); //function calls admminmenu()
                return;
            }
            else{ //otherwise runs this
                user_check = 1; //sets user_check to 1
                CustomerMenu(); //function call CustomerMenu()
                return;
            }

        }
    }
    fclose(userlist);
    red(); printf("\n\t[ Invalid Username. ]"); reset();
    delaynext(1000);
    LoginMenu();
}

//function for Sign-up
void SignUp(FILE * userlist, FILE * passlist){
    char userhold[30], passhold[30],bannedname[30];
    int count,i;
    strcpy(bannedname,"admin");

    //user prompt
    blue();printf("\n\tSIGN-UP\n");
    printf("\t----------------------------------------------------"); reset();
    printf("\n\tUsername: ");
    scanf(" %29s",username);

    for(i = 0 ; i < 30 ; i++){ //for loop to check if spaces are present
        if(username[i] == ' '){
            red();
            printf("\n\t[ Invalid Username: Spaces aren't allowed ]");
            reset();
            printf("\n\tUsername: ");
            scanf(" %29s",username); 
            i = 0;
        }
    }

    userlist = fopen("username.txt", "r");

    while(fscanf(userlist, "%s",userhold) == 1){//loop to check if the inputted username is already taken and asks the user to input another one
        while(strcmp(userhold,username) == 0){
            red(); printf("\t[ Username \"%s\" is already taken. ]", username); reset();
            printf("\n\tUsername: ");
            scanf("%s",username);
        }
        //for-loop to check if the words used in the username is banned or not
        for (count = 0 ; count < 5 ; count ++){
            if(username[count] == bannedname[count]){
                if (count == 4){
                    red(); printf("\t[ Username \"%s\" is invalid. ]",username); reset();
                    printf("\n\tUsername: ");
                    scanf("%s",username);
                }
            }else {
                break;
            }
        }
    }

    userlist = fopen("username.txt","a"); //opens username.txt as append
    fprintf(userlist,"%s\n",username); //adds the inputted username to the text file
    fclose(userlist); //closes the file

    //prompt for password
    printf("\tPassword: ");
    scanf(" %29s",password); 

    for(i = 0 ; i < 30 ; i++){ //for loop to check if spaces are present
        if(password[i] == ' '){
            red();
            printf("\n\t[ Invalid Password: Spaces aren't allowed ]");
            reset();
            printf("\n\tPassword: ");
            scanf(" %29s",password);
            i = 0;
        }
    }

    passlist = fopen("password.txt","a"); //opens password.txt as append
    fprintf(userlist,"%s\n",password); //adds the inputted password to the text file
    fclose(passlist); //closes the files

    green(); printf("\t[ Sign-up successful! ]"); reset();
    delaynext(1000);
    LoginMenu(); //returns to LoginMenu()
}

//function for the Admin Sign-up
void AdminSignUp(FILE * userlist, FILE * passlist) {
    char userhold[30], passhold[30],requiredname[30];
    int count,i=0, check=0;
    strcpy(requiredname,"admin");
    blue();
    printf("\n\tADMIN SIGN-UP\n");
    printf("\t----------------------------------------------------");
    reset();
    printf("\n\tUsername: ");
    scanf("%29s",username);

    for(i = 0 ; i < 30 ; i++){ //for loop to check if spaces are present
        if(username[i] == ' '){
            red();
            printf("\n\t[ Invalid Username: Spaces aren't allowed ]");
            reset();
            printf("\n\tUsername: ");
            scanf("%29s",username); //scanf function that will terminate upon reading a newline character
            i = 0;
        }
    }

    userlist = fopen("username.txt", "r"); //opens username.txt for reading

    while (check==0) {
        //for-loop that checks if there is "admin" at the beginning of the username
        for (count = 0 ; count < 5 ; count ++) {
            if(username[count] == requiredname[count]){
                if (count == 4) {
                    check=1;
                    break;
                }
            } else {
                red(); printf("\t[ Username should have \"admin\" in the beginning. ]"); reset(); //outputs error message if there is no "admin"
                printf("\n\tUsername: ");
                scanf("%29s",username);
                check=0;
            }
        }
        //loop that goes through the file
        while(fscanf(userlist, "%s",userhold)==1){
            while(strcmp(userhold,username) == 0){ //loop that iterates until username and userhold are the same
                red(); printf("\t[ Username \"%s\" is already taken. ]", username); reset(); //if the same username is found, outputs error message
                printf("\n\tUsername: ");
                scanf("%29s",username);
                check=0;
            }
            if (strcmp(userhold,username) != 0)  {
                check=1;
                break;
            }
        }
    }
    fclose(userlist);

    
    userlist = fopen("username.txt","a"); //opens username.txt as append
    fprintf(userlist,"%s\n",username); //adds the username to the file
    fclose(userlist);

    printf("\tPassword: "); //prompts the user for a password
    scanf("%29s",password);

    for(i = 0 ; i < 30 ; i++){ //for loop to check if spaces are present
        if(password[i] == ' '){
            red();
            printf("\n\t[ Invalid Password: Spaces aren't allowed ]");
            reset();
            printf("\n\tPassword: ");
            scanf("%29s",password); 
            i = 0;
        }
    }

    passlist = fopen("password.txt","a"); //opens password.txt as append
    fprintf(userlist,"%s\n",password); //adds password to the file
    fclose(passlist);

    green(); printf("\t[ Sign-up Successful! ]"); reset(); //outputs sign-up successful message
    delaynext(1000);
    LoginMenu(); //returns to LoginMenu()
}

/* ---------------------------------------------------CART FUNCTIONS--------------------------------------------------------------- */

//function for the customer menu
void CustomerMenu(void){
    discount=total=points=0;
    headinv = readinv(); //sets as the header of the linked list for inventory
    headcart = readCart(); //sets as header of the linked list for cart
    int back=1;

    //code sequence of cart menu
    while(back==1) {
        system("clear || cls");
        blue();
        if (user_check==0) //checks if user is a guest user or registered user
            printf("\n\tWelcome!\n"); //outputs message if guest
        else
            printf("\n\tWelcome back, %s!\n", username); //outputs message if user
        reset();
        compare(); //function calls compare()
        showCart(); //function calls showCart()

        //outputs list of options for cart menu
        printf("\n\n\t<1> Add Items\n");
        printf("\t<2> Remove Items\n");
        if (user_check==0)
            printf("\t<3> Apply Points\n");
        else
            printf("\t<3> Apply Points (%.2f)\n", (total/10)-discount); //outputs amount of points in the option
        printf("\t<4> Checkout\n");
        printf("\t<5> Help\n");
        printf("\t<6> Log-Out\n");
        printf("\t<7> Exit\n");
    
        printf("\n\t> Enter response: ");
        scanf("%d", &opt); //prompt for option
        
        //switch case for option
        switch(opt){
            case 1: system("clear || cls"); AddItems();  break;//function calls AddItems()
            case 2: system("clear || cls"); RemoveItems();  break;//function calls RemoveItems()
            case 3: system("clear || cls"); ApplyPoints();  break;//function ApplyPoints()
            case 4: system("clear || cls"); Receipt(); writeCart(); writeinv();  break;//function calls Receipt(), writeCart(), writeinv()
            case 5: helpCart();  break;//function calls helpCart()
            case 6: printf("\n\tThanks for shopping with us \\(^-^)/!\n"); delaynext(1000); writeCart(); LoginMenu(); back=0; break; //outputs exit message and function calls writeCart() goes back to LoginMenu()
            case 7: printf("\n\tThanks for shopping with us \\(^-^)/!\n"); printf("\n\t[ Exiting program. ]"); writeCart(); exit(0); //outputs exit message and terminates program
            default: printf("\n\t[ Invalid option, please try again. ]"); delaynext(1000); //outputs error message if invalid input
        }
    }
}

//function that display the help information for cart
void helpCart(void) {
    int opt = 1;
    //outputs help information
    while (opt!=0) {
    //page 1 of the help info
        if(opt==1) {
            system("clear || cls");
            blue();
            printf("\n\tH E L P \n");
            printf("\t----------------------------------------------------\n");
            reset();
            green();
            printf("\t[ Add Items ]\n");
            reset();
            printf("\tYou can use this to add items to your cart. You first\n");
            printf("\tneed to select the item code of the item that you want\n");
            printf("\tto add then input the quantity that you want. The item\n");
            printf("\twill then be automatically added to your cart.\n\n");
            green();
            printf("\t[ Remove Items ]\n");
            reset();
            printf("\tHere you can remove an item from your cart by simply\n");
            printf("\tentering the item code of the item you want to remove\n");
            printf("\tand it will be automatically be removed.\n\n");
            green();
            printf("\t[ Apply Points ]\n");
            reset();
            printf("\tIf you are a registered user, you can apply points to\n");
            printf("\tavail discounts. You get 1 point per 10 pesos of\n");
            printf("\tpurchase. 1 point is equivalent to 1 peso. Selecting\n");
            printf("\tthis option will apply the discount to your current cart.\n\n");
            printf("\tPress 2 to go to page 2, Press 0 to exit: ");
        }
        else if (opt==2) {
            system("clear || cls");
            blue();
            printf("\n\tH E L P \n");
            printf("\t----------------------------------------------------\n");
            reset();
            green();
            printf("\t[ Checkout ]\n");
            reset();
            printf("\tThis will proceed you to the checkout tab where you\n");
            printf("\twould checkout your cart. You will be shown a purchase\n");
            printf("\treceipt with the total costs included. Points are also\n");
            printf("\tautomatically added for your next purchase.\n\n");
            green();
            printf("\t[ Log-Out ]\n");
            reset();
            printf("\tSelecting this will log you out of your account and\n");
            printf("\tredirect you to the login screen. Don't worry all items\n");
            printf("\tin cart are saved. If there are unavailable items in\n");
            printf("\tyour existing cart the next time you log in, they will be\n");
            printf("\tautomatically removed.\n\n");
            green();
            printf("\t[ Exit ]\n");
            reset();
            printf("\tExits you from the store, which ends the program.\n\n");
            printf("\tPress 1 to go back to page 1, Press 0 to exit: ");
        }
        else if (opt<0 || opt>2) {
            red(); printf("\n\t[ Invalid input. ]\n"); reset();
            delaynext(1000);
            return;
        }
        scanf("%d", &opt);  
    }
}   

//reads the cart for the linked list
struct item *readCart(void) {   
    struct item *temp=malloc(sizeof(struct item)); //creating node for the cart items

    //if-else statement to open the file of the cart of the user
    if (user_check==0) { //checks if user is a guest or registered
        return NULL; //no file to read if guest
    }
    else if (user_check==1) { //otherwise if user is registered
        char fname[MAX]; //variable to store the user's username
        strcpy(fname, username);
        strcat(fname, ".txt");
        user_cart=fopen(fname, "r"); //opens the file with the user's username
    }
    if (user_cart==NULL) //if cart is empty return NULL
        return NULL;

    fscanf(user_cart, "%f", &total); //reads the total amount the user purchased
    points=total/10; //divides the total by 10 to get the amount of points
    if(feof(user_cart)) { //checks if cart is empty
        return NULL;
    }

    //loop that reads the content of the user's cart and assigns it to the linked list
    while ( fscanf(user_cart, "%d %19s %f %d", &temp->code, temp->name, &temp->price, &temp->stock) != EOF){
        if (headcart==NULL)
            headcart=tailcart=temp;
        else { 
            tailcart->link=temp;
            tailcart=temp;
        }
        temp=malloc(sizeof(struct item));
    }
    tailcart->link=NULL;
    free(temp);
    fclose(user_cart);
    return headcart;
}

//function for comparing the stocks of the user cart and inventory (for availability checking)
void compare (void) {
    struct item *currinv, *currcart, *prevcart; //declaring struct pointers for cart and inventory
    currcart=prevcart=headcart;

    if(currcart==NULL) { //checks if the current pointed node is empty
        return;
    }
    while(currcart!=NULL) { //if pointed node is not empty
        currinv=headinv;
        while(currinv!=NULL) { //checks if item in inventory is still available
            if (currinv->code==currcart->code){ //checks if item has the same code
                if (currcart->stock>currinv->stock) { //checks if the item stock in cart is greater than the stock in inventory
                    red();
                    printf("\n\t[ Item %d removed. Insufficient stocks available. ]", currcart->code); //outputs message that item is insufficient
                    reset(); 
                    //removes item from cart
                    if(currcart==headcart){
                        if(headcart==tailcart)
                            tailcart=currcart->link; 
                        headcart=currcart->link;
                    }
                    else 
                        prevcart->link=currcart->link;
                }
                break;
            }
            currinv=currinv->link;
        }
        if (currinv==NULL) { //item is unavailable
            red();
            printf("\n\t[ Unavailable item %d removed. ]", currcart->code); //outputs message that item is removed
            reset();
            //removes item from inventory
            if(currcart==headcart)
                headcart=currcart->link;
            if(currcart==tailcart) {
                prevcart->link=NULL;
                tailcart=prevcart; 
            }
            else
                prevcart->link=currcart->link;
        }
        prevcart=currcart;
        currcart=currcart->link;
    }
    free(currcart);
}

//function that displays the contents of user cart
void showCart(void){
    struct item *curr; //declaring struct pointer for current node
    totalr = 0; //store the discounted price (if ever)
    curr = headcart; //assigns curr to the head node
    blue();
    //outputs formatting
    printf("\n\tYOUR CART \n");
    printf("\t----------------------------------------------------");
    reset();
    if(curr==NULL) { //checks if cart is empty
        printf("\n\tNo items to see here yet..."); //outputs message that there are no items yet
        return;
    }
    //loop that outputs the contents of the user cart
    while (curr!=NULL) {
        printf("\n\t%04d %-30s %dx%-8.2f %.2f",curr->code, curr->name, curr->stock, curr->price, curr->price * curr->stock); //outputs the content of user cart line by line
        totalr += (curr->price * curr->stock); //adds up the total amount of the items
        curr = curr->link; //goes to next node
    }
    totalr-=discount; //subtracts the discount from the total amount
    puts("");
    blue();
    printf("\t----------------------------------------------------\n");
    printf("\t                                DISCOUNT:-%10.2f\n", discount); //outputs the discount amount
    printf("\t                                   TOTAL: %10.2f\n", totalr); //outputs puts the total amout with the discount applied
    reset();
}

//function for adding items to the cart
void AddItems(void) {
    int icode, istock; //variables for input code and quantity desired
    struct item *new=malloc(sizeof(struct item)); new->link=NULL; //creating a new node
    struct item *currinv, *currcart; //declaring struct pointers for current node for cart and inventory
    currinv=headinv;
    currcart=headcart;
    blue();
    printf("\n\tA D D  I T E M\n");
    reset();
    showInv(); //function calls showInv()

    do {
        printf("\n\t> Input code of the item to be added: ");
        fflush(stdin);
        check=scanf("%d", &icode); //prompts for the code of the item to be added
        } while (check==0); //checks if input is valid
    do {
        printf("\n\t> Input quantity: ");
        fflush(stdin);
        check=scanf("%d", &istock); //prompts for the quantity of the item
        } while (check==0); //checks if input is valid

    if (istock<=0){
        red(); printf("\n\t[ Cannot add item. Invalid quantity. ]"); reset(); //output error message that item cannot be added
        delaynext(1000);
        return;
    }
    //loops that adds the item to the cart
    while (currinv!=NULL) { //goes through the inventory
        if(currinv->code == icode) { //finds the same inputted code in the inventory
            if (istock <= currinv->stock) { //checks if the inputted quantity is less than or equal to the stock in inventory
                while(currcart!=NULL) { //goes through cart
                    if (currcart->code==icode) { //finds the same inputted code in the cart
                        if (((currcart->stock)+istock)<=currinv->stock) { //checks if the item stock in the cart with the added desired quantity is less than or equal to the stock in the inventory
                            currcart->stock+=istock; //adds the inputted quantity to stock in the cart
                            currcart=NULL;
                            green(); printf("\n\t[ Added to cart succesfully! ]\n"); reset(); //outputs message that the item has been added to cart
                            delaynext(1000);
                            return;
                        }
                        else { //if inputted quantity is more than the stocks in inventory
                            red(); printf("\n\t[ Cannot add item. Insufficient stocks available. ]"); reset(); //output error message that item cannot be added
                            delaynext(1000);
                            return;
                        }
                    }
                    else
                        currcart=currcart->link;
                }
                //adds item to the cart
                new->code=currinv->code;
                strcpy(new->name, currinv->name);
                new->price=currinv->price;
                new->stock=istock;
                if (headcart==NULL) {
                    headcart=tailcart=new;}
                else {
                    tailcart->link=new;
                    tailcart=new;
                }
                tailcart->link=new=currinv=currcart=NULL;
                green(); printf("\n\t[ Added to cart succesfully! ]\n"); reset(); //outputs message that item has been added to cart
                delaynext(1000);
                return;
            }
            else { //if inputted quantity is more than the stocks in inventory
                red(); printf("\n\t[ Cannot add item. Insufficient stocks available. ]"); reset(); //outputs error message
                delaynext(1000);
                return;
            }
        }
        else 
            currinv=currinv->link;
    }
    red(); printf("\n\t[ Cannot add item. Item does not exist. ]"); reset(); //outputs if item code does not exist
    delaynext(1000);
}

//function to remove items in user cart
void RemoveItems(void) {
    int icode, istock; //variables for input code and desired quantity
    struct item *curr, *prev; 
    blue();
    printf("\n\tR E M O V E  I T E M\n");
    reset();
    showCart(); //function call showCart()
    curr=prev=headcart;
    if(curr==NULL) { //check if curr is empty
        printf("\t[ No item to delete yet ]"); 
        return;
    }
    
    do {
        printf("\n\t> Input code of product to be deleted: ");
        fflush(stdin);
        check=scanf("%d", &icode); //prompt for item code desired
    } while (check==0); //checks if input is valid

    //loop that deletes the item
    while (curr!=NULL) { //while curr is not empty
        if(curr->code==icode) { //finds the same inputted code in the cart
            //deletes the inputted item if found
            if(curr==headcart) {
                if(headcart==tailcart){
                    tailcart=curr->link;
                }
                headcart=curr->link;
            }
            else
              prev->link=curr->link;
            free(curr);
            green(); printf("\n\t[ Deleted item with code %d. ]", icode); reset(); //outputs message that item has been removed
            delaynext(1000);
            if(discount>0) {
                points=discount;
                discount=0;
            }
            return;
        }   
        else {
          prev=curr;
          curr=curr->link;
        }
    }
    //if inputted code doesn't exist
    red(); printf("\t[ No item with code %d found ]", icode); reset();
    delaynext(1000);
}

//function to apply points on the total amount
void ApplyPoints(void) {
    if(user_check == 1){ //checks if user is registered
        if (points < totalr) { //checks if points is less than the total amount
            discount = points; //assigns the points to the discount
            points = 0;
        }
        else if(points>totalr) { //checks if points is more than the total amount
            points-=totalr; //subtracts total amount to points
            discount = totalr;
        }
        else { //case that points is equal
            discount = points;
            points = 0;
        }
    }
    else{ //if user is not logged in
        red();
        printf("\n\n\t[ You must be logged in to use points. ]\n"); //outputs message that user must be logged in
        reset();
        delaynext(1000);
    }
}

//function to display the receipt purchase
void Receipt(void) {
    struct item *currcart, *currinv; //struct pointers to navigate the linked list for cart and inventory
    currcart = headcart;
    currinv = headinv;
    system("clear || cls");
    blue();
    //outputs receipt formatting
    printf("\n\t====================================================\n");   
    printf("\t                       RECEIPT \n");
    printf("\t====================================================\n\n");
    printf("\tCODE    NAME                             AMOUNT(PHP)\n");
    printf("\t----------------------------------------------------\n");
    reset();
    if(currcart==NULL){  //checks if cart is empty
        printf("\t[ Try adding items before checking out... ]\n"); //outputs message that cart is empty
        printf("\n\tPress enter to continue...");
        getc(stdin);
        return;
    }
    while (currcart != NULL) { //if cart is not empty
        printf("\n\t%04d %-30s %dx%-8.2f %.2f",currcart->code, currcart->name, currcart->stock, currcart->price, currcart->price * currcart->stock); //outputs the contents of the cart line by line
        currinv = headinv;
        while (currinv != NULL) { //check if item in inventory is not empty
            if(currinv->code==currcart->code) { //finds the same code in cart in the inventory
                currinv->stock-=currcart->stock; //decreases the stock accordingly
                break; }
            else
                currinv = currinv->link;
        }
        currcart = currcart->link;
        free(headcart);
        headcart = currcart;
    }
    puts("");
    blue();
    printf("\t----------------------------------------------------\n");
    printf("\t                                DISCOUNT:-%10.2f\n", discount); //outputs total discount (based on points)
    printf("\t                                   TOTAL: %10.2f\n", totalr); //outputs the discounted total
    reset();
    printf("\n\n\n\tThanks for shopping with us \\(^-^)/!\n"); //outputs exit message
    total+=totalr;
    points=total/10; 
    printf("\n\tPress enter to continue...");
    fflush(stdin);
    getc(stdin);
}

//function that writes the contents of the cart to a file
void writeCart(void){
    if (user_check == 0) { //checks if user is logged in
        return;
    }
    else{ //if so writes the username as the filename
        char fname[MAX];
        strcpy(fname, username);
        strcat(fname, ".txt");
        user_cart=fopen(fname, "w");
    }
    //checks if file has been created
    if (user_cart==NULL) {
        red(); printf("\t[ Couldn't Create File ]\n"); reset();
        delaynext(1000);
        return;
    }

    struct item *curr;
    fprintf(user_cart, "%f", total-(discount*10)); //writes on files the total spending
    if (headcart==NULL) {
        fclose(user_cart);
    }
    curr=headcart;
    while (curr!=NULL) {
        fprintf(user_cart, "\n%d %-30s %.2f %d",curr->code, curr->name, curr->price, curr->stock); //writes on file the contents of the cart the user made upon exiting/checkout
        curr = curr->link;
    }
    fclose(user_cart);
}

/* ---------------------------------------------INVENTORY FUNCTIONS---------------------------------------------------------- */

//function to access the admin menu
void adminmenu(void) {
    //calls the readinv function and assigns returned value to headinv
    headinv = readinv();
    int back=1;

    while(back==1) {
        system("clear || cls");
        blue();
        printf("\n\tWelcome back, %s!\n", username);
        reset();
        //calls showinv to output list of available items in inventory
        showInv();
        printf("\n\n\t<1> Add Product Listing\n");
        printf("\t<2> Update Product Listing\n");
        printf("\t<3> Delete Product Listing\n");
        printf("\t<4> Help\n");
        printf("\t<5> Log-out\n");
        printf("\t<6> Exit\n");

        //for user input
        do {
            printf("\t> Input response: ");
            fflush(stdin);
            check=scanf("%d", &opt);
        } while (check==0);

        //switch-case to redirect user according to input
        switch (opt){
            case 1: system("clear || cls"); addp();  break;//calls addp and go back to adminopt
            case 2: system("clear || cls"); updatep(); break;//calls updatep and go back to adminopt
            case 3: system("clear || cls"); deletep(); break;//calls deletep and go back to adminopt
            case 4: helpAdmin();  break;//calls helpAdmin() for instructions
            case 5: writeinv(); LoginMenu(); back=0; break; //calls writeinv and proceeds to Login menu
            case 6: writeinv(); printf("\n\t[ Exiting program. ]"); exit(0); //calls writeinv and exits program
            default: printf("\n\t[ Option does not exist. Please try again. ]");  //for invalid input
        }
    }
}

void helpAdmin(void){
    int opt = 1;
    //outputs help information
    while (opt!=0) {
    //page 1 of the help info
        if(opt==1) {
        system("clear || cls");
        blue();
        printf("\n\tH E L P \n");
        printf("\t----------------------------------------------------\n");
        reset();
        green();
        printf("\t[ Add Product Listing ]\n");
        reset();
        printf("\tYou can use this to add a product to your shop. You first\n");
        printf("\tneed to provide the name of the item (30 char) and then the\n");
        printf("\tprice and lastly, the stock available. The item code will be\n");
        printf("\tthe next number from the last one.\n\n");
        green();
        printf("\t[ Update Product Listing ]\n");
        reset();
        printf("\tHere you can update an item from your cart by simply\n");
        printf("\tentering the item code of the item you want to remove\n");
        printf("\tand entering the new information. The item code will\n");
        printf("\tbe the same.\n\n");
        green();
        printf("\t[ Remove Product Listing ]\n");
        reset();
        printf("\tHere you can remove a product from your shop by simply\n");
        printf("\tentering the item code of the item you want to remove\n\n");
        printf("\tPress 2 to go back to page 2, Press 0 to exit: ");
        }   
        else if (opt==2) {
        system("clear || cls");
        blue();
        printf("\n\tH E L P \n");
        printf("\t----------------------------------------------------\n");
        reset();
        green();
        printf("\t[ Log-Out ]\n");
        reset();
        printf("\tSelecting this will log you out of your account and\n");
        printf("\tredirect you to the login screen. Don't worry all product\n");
        printf("\tlistings you have added will be saved.\n\n");
        green();
        printf("\t[ Exit ]\n");
        reset();
        printf("\tExits you from the store, which ends the program.\n\n");
        printf("\tPress 1 to go back to page 1, Press 0 to exit: ");
        }
        else if (opt<0 || opt>2) {
            red(); printf("\n\t[ Invalid input. ]\n"); reset();
            delaynext(1000);
            return;
        }
        scanf("%d", &opt);  
    }
}   

void addp(void) {
    //allocates memory for new node
    struct item *new=malloc(sizeof(struct item));
    blue(); 
    printf("\n\tA D D  P R O D U C T\n");
    printf("\t----------------------------------------------------\n");
    reset();
    //input product name in new node
    do {
        printf("\t> Input name: ");
        fflush(stdin);
        check=scanf("%19s", new->name);
    } while (check==0);
    //input product price in new node
    do {
        printf("\t> Input price: ");
        fflush(stdin);
        check=scanf("%f", &new->price);
    } while (check==0);
    //input product stocks in new node
    do {
        printf("\t> Input stock: ");
        fflush(stdin);
        check=scanf("%d", &new->stock);
    } while (check==0);
    
    new->link=NULL;
    //if there is no existing product yet, the item code will be 1 and address of new will be assigned to headinv and tailinv
    if (headinv==NULL) {
        new->code=1;
        headinv=tailinv=new;
        }
    //else, item code will be +1 to the tail's code and address of new will be the tail address
    else {
        new->code=(tailinv->code)+1;
        tailinv->link=new;
        tailinv=new;
        
    }
    tailinv->link=NULL; new=NULL; 
    green(); printf("\n\t[ Added new product ]"); reset();
    delaynext(1000);
}

void updatep(void) {
    //declarations
    int code;
    struct item *currinv;
    //start at the first node
    currinv=headinv;

    blue();
    printf("\n\tU P D A T E  P R O D U C T\n");
    printf("\t----------------------------------------------------\n");
    reset();
    
    //if the currinv is null, the list is empty
    if(currinv==NULL) {
        printf("\n\tNo item to update yet...\n");
        return;
    }
    //else, ask for product code
    do {
            printf("\n\t> Input code of product to be updated: ");
            fflush(stdin);
            check=scanf("%d", &code);
        } while (check==0);

    //goes through all the nodes/products
    while (currinv!=NULL) {
        //if the inputted code is found, ask for the updated information
        if(currinv->code==code) {
            do {
                printf("\t> Input new name: ");
                fflush(stdin);
                check=scanf("%30s", currinv->name);
            } while (check==0);
            do {
                printf("\t> Input new price: ");
                fflush(stdin);
                check=scanf("%f", &currinv->price);
            } while (check==0);
            do {
                printf("\t> Input new stock: ");
                fflush(stdin);
                check=scanf("%d", &currinv->stock);
            } while (check==0);
            green(); printf("\n\t[ Updated item %d ]", code); reset();
            delaynext(1000);
            return;
        }   
        //else, go to the next node/product
        else 
            currinv=currinv->link;
    }
    red(); printf("\n\t[ No item with code %d found ]", code); reset();
    delaynext(1000);
}

void deletep(void) {
    //declarations
    int code;
    struct item *currinv, *previnv;
    //start at the first node
    currinv=previnv=headinv;

    blue();
    printf("\n\tD E L E T E  P R O D U C T\n");
    printf("\t----------------------------------------------------\n");
    reset();

    //if the currinv is null, the list is empty
    if(currinv==NULL) {
        printf("\tNo item to delete yet...");
        return;
    }
    //else, ask for the item code to be deleted
    do {
        printf("\n\t> Input code of product to be deleted: ");
        fflush(stdin);
        check=scanf("%d", &code);
    } while (check==0);

    //goes through all the nodes/products
    while (currinv!=NULL) {
        //if the code is found...
        if(currinv->code==code) {
            //and if it's the first node, update the second to be the first node
            if(currinv==headinv) {
                //and if it's the only node, tailinv will be null
                if(headinv==tailinv)
                    tailinv=currinv->link;
              headinv=currinv->link;
            }
            //else the prev node will be point to the next node after currinv
            else
              previnv->link=currinv->link;
            //free the memory from currinv
            free(currinv);
            green(); printf("\n\t[ Deleted item %d ]", code); reset();
            delaynext(1000);
            return;
        } 
        //else, go to next node 
        else {
          previnv=currinv;
          currinv=currinv->link;
        }
    }
    
    red(); printf("\t[ No item with code %d found ]", code); reset();
    delaynext(1000);
}

void showInv(void){
    //declarations
    struct item *currinv;
    //point currinv to the first item
    currinv = headinv;
    printf("\n\tAVAILABLE ITEMS\n");
    printf("\t--------------------------------------------------");
    printf("\n\tCODE      ITEM NAME                 PRICE    STOCK");
    printf("\n\t--------------------------------------------------");
    //if currinv is NULL, list is empty
    if(currinv==NULL) {
        printf("\n\tNo items to see here yet...");
    }
    //goes through all the nodes/product
    while (currinv!=NULL) {
        //print the product listing only if the stocks is over 0
        if(currinv->stock>0)
            printf("\n\t%04d %-30s %-8.2f %d",currinv->code, currinv->name, currinv->price, currinv->stock);
        currinv = currinv->link;
    }
    puts("");
    
}

struct item *readinv(void) {
    //allocate memory to temp
    struct item *temp=malloc(sizeof(struct item));
    //assign pointer to opened inventory text for reading
    inventory=fopen("inventory.txt", "r"); 
    //if inventory null, no previous listings yet
    if (inventory==NULL)
        return NULL;
    //goes through all the exiting product listings
    while ( fscanf(inventory, "%d %30s %f %d", &temp->code, temp->name, &temp->price, &temp->stock) !=EOF) {
        //assigns the pointer for the first node
        if (headinv==NULL)
            headinv=tailinv=temp;
        //assigns pointer for the succeeding nodes
        else {
            tailinv->link=temp;
            tailinv=temp;
        }
        //allocate a new memory for temp
        temp=malloc(sizeof(struct item));
    }
    //assigns NULL to the link member of the last node
    tailinv->link=NULL;
    free(temp);
    //closes inventory
    fclose(inventory);
    //returns address of headinv
    return headinv;
}

void writeinv(void){
    //assign pointer to opened inventory text for writing
    inventory=fopen("inventory.txt", "w");

    if (inventory==NULL) {
        red(); printf("[ Couldn't Create File ]\n"); reset();
        return;
    }
    struct item *currinv;
    if (headinv==NULL) {
        fclose(inventory);
        remove("inventory.txt");
    }
    currinv=headinv;
    while (currinv!=NULL) {
        fprintf(inventory, "%d %-30s %.2f %d\n",currinv->code, currinv->name, currinv->price, currinv->stock);
        currinv = currinv->link;
    }
    //close the file
    fclose(inventory);
}

// ------------------------------ OTHER FUNCTIONS ------------------------------ //

void delaynext(long milli) {
    clock_t end, current = clock();
    for( end = current + milli; current < end; current = clock() );
}