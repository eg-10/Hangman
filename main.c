
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include<stdlib.h>
#include<time.h>

int loadAllHints();
char *strupr(char *str);
void genAlert(char message[]);
void on_btn_rehint_clicked();
void on_btn_main_menu_clicked();

GtkWidget *window;

GtkWidget *g_lbl_hint;
GtkWidget *g_btn_submit;
GtkWidget *g_lbl_answer;
GtkWidget *g_txt_letter;
GtkWidget *g_btn_start;
GtkWidget *g_btn_rehint;
GtkWidget *g_btn_main_menu;
GtkWidget *g_img_init;
GtkWidget *g_img_1;
GtkWidget *g_img_2;
GtkWidget *g_img_3;
GtkWidget *g_img_4;
GtkWidget *g_img_5;


char allHints[50][100];
char allAnswers[50][50];
int numHints=0;

char currAns[50];
char currHint[50];

char encrAns[50];

int quitGame=0;

int rehinted=0;

int gameStage=0;

char enteredLetters[50];
int numLetters=0;

void init_game(int argc, char *argv[])
{
    quitGame=0;
    numLetters=0;
    rehinted=0;
    gameStage=0;
    for(int i=0;i<50;i++){
        enteredLetters[i]=' ';
        encrAns[i]=' ';
    }
    GtkBuilder      *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "window_main1.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    // get pointers to the widgets
    g_lbl_hint = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_hint"));
    g_btn_submit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_submit"));
    g_btn_start = GTK_WIDGET(gtk_builder_get_object(builder, "btn_start"));
    g_txt_letter = GTK_WIDGET(gtk_builder_get_object(builder, "txt_letter"));
    g_lbl_answer=GTK_WIDGET(gtk_builder_get_object(builder,"lbl_answer"));
    g_btn_rehint=GTK_WIDGET(gtk_builder_get_object(builder,"btn_rehint"));
    g_btn_main_menu=GTK_WIDGET(gtk_builder_get_object(builder,"btn_main_menu"));
    g_img_init= GTK_WIDGET(gtk_builder_get_object(builder, "img_init"));
    g_img_1 = GTK_WIDGET(gtk_builder_get_object(builder, "img_1"));
    g_img_2 = GTK_WIDGET(gtk_builder_get_object(builder, "img_2"));
    g_img_3 = GTK_WIDGET(gtk_builder_get_object(builder, "img_3"));
    g_img_4 = GTK_WIDGET(gtk_builder_get_object(builder, "img_4"));
    g_img_5 = GTK_WIDGET(gtk_builder_get_object(builder, "img_5"));

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main(); 
}


int main(int argc, char *argv[])
{
    numHints=loadAllHints();
    do{
        init_game(argc, argv);
    }while(quitGame);
    return 0;
}

void showGame()
{
    gtk_widget_show(g_lbl_hint); 
    gtk_widget_show(g_btn_rehint);
    gtk_widget_show(g_btn_submit);
    gtk_widget_show(g_lbl_answer);
    gtk_widget_show(g_txt_letter);
    gtk_widget_show(g_btn_main_menu);
    gtk_widget_show(g_lbl_answer);
    gtk_widget_hide(g_btn_start);
    gtk_widget_hide(g_img_init);
}

int confirmExit()
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new(GTK_WINDOW(window), 
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, 
                                  GTK_BUTTONS_YES_NO,
                                  "Are you sure you want to quit?" );
    gtk_window_set_title(GTK_WINDOW(dialog), "Quit Game");
    int quit=gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    if (quit==GTK_RESPONSE_YES)
        return 1;
    else
        return 0;
}

int winMessage()
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new(GTK_WINDOW(window), 
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, 
                                  GTK_BUTTONS_YES_NO,
                                  "Congrats, You Won!!\n Play Again?" );
    gtk_window_set_title(GTK_WINDOW(dialog), "Congratulations!");
    int playAgain=gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    if (playAgain==GTK_RESPONSE_YES)
        return 1;
    else
        return 0;
}

int loseMessage(){

    GtkWidget *dialog;
    dialog=gtk_message_dialog_new(GTK_WINDOW(window), 
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, 
                                  GTK_BUTTONS_YES_NO,
                                  "You lost all your lives!!\n Play Again with a new hint?" );
    gtk_window_set_title(GTK_WINDOW(dialog), "You Lost!!");
    int playAgain=gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    if (playAgain==GTK_RESPONSE_YES)
        return 1;
    else
        return 0;

}

void setStage()
{
    int playAgain;
    if(strcmp(encrAns,currAns)==0){
        if(rehinted)
            return;
        playAgain=winMessage();
        if(playAgain)
            on_btn_rehint_clicked();
        else
            on_btn_main_menu_clicked();
        return;
    }
    if (gameStage==0){
        gtk_widget_hide(g_img_2);
        gtk_widget_hide(g_img_3);
        gtk_widget_hide(g_img_4);
        gtk_widget_hide(g_img_5);
        gtk_widget_show(g_img_1);
    }
    else if(gameStage==1){
        gtk_widget_hide(g_img_1);
        gtk_widget_show(g_img_2);
    }
    else if(gameStage==2){       
        gtk_widget_hide(g_img_2);
        gtk_widget_show(g_img_3);
    }
    else if(gameStage==3){
        gtk_widget_hide(g_img_3);
        gtk_widget_show(g_img_4);
    }
    else if(gameStage==4){ 
        gtk_widget_hide(g_img_4);
        gtk_widget_show(g_img_5);
        playAgain=loseMessage();
        if(playAgain)
            on_btn_rehint_clicked();
        else
            on_btn_main_menu_clicked();

    }
    
}

int loadAllHints(){        /*Abhir Raj - load all hints from  notepad to char array */
    
    int i=0;
    FILE *hints,*answers;
    answers=fopen("answers","r");
    hints=fopen("hints","r");
    while(!(feof(answers))){

        fscanf(answers," %[^\n]s",allAnswers[i]);
        strupr(allAnswers[i]);
        i++;
    }
    fclose(answers);
    i=0;
    while(!(feof(hints))){

        fscanf(hints," %[^\n]%*c",allHints[i]);
        i++;
    }
    fclose(answers);
    return ++i;
}

int genHint(void){         /*Abhir Raj - pick a random hint from array of hints*/
    
    srand(time(NULL));
    int upper=numHints-1;
    int lower=0;
    return ((rand()%(upper-lower))+lower);
    
} 

void dispHint(void){    /*EG Harshal - use genHint and put the generated hint in label*/
    
    int i=genHint();
    gtk_label_set_text((GtkLabel *)g_lbl_hint,(const gchar *)allHints[i]);
    strcpy(currAns,allAnswers[i]);    
} 

void dispAnswer(void){      /*EG Harshal - use display the encr. ans in label*/
    
    int i;
    for(i=0; i<strlen(currAns);i++){
        if(isalpha(currAns[i])){
            encrAns[i]='_';
        }
        else
            encrAns[i]=currAns[i];
    }
    encrAns[i]='\0';
    gtk_label_set_text ((GtkLabel *)g_lbl_answer,(const gchar *)encrAns);

} 

int isCharRepeated(char c)  /*Raj - Checks if char has already been entered by user*/ 
{   
    int i,r=0;
    for(i=0;i<numLetters;i++)
    {
        if(enteredLetters[i]==c)
        {
            r++;
        }
    }
    if(r==0)
        return 0;
    
    else 
        return 1;
}

int isRightChar(char c){     /*Raj- Checks if the char is present in answer*/
    int i,r=0;

    for(i=0;i<strlen(currAns);i++)
    {
        if(currAns[i]==c)
        {
            r++;
        }
    }
    if(r==0)
        return 0;
    
    else 
        return 1;
}



void processChar(char c){               /*EG Harshal - uses isCharRepeated and isRightChar to check char and
                                        displays char in label if correct else takes one life from user */
    rehinted=0;
    c=toupper(c);
    int i;
    if(isCharRepeated(c)){
        genAlert(" You've already entered that Letter! ");
        return;
    }
    else{
        enteredLetters[numLetters]=c;
        numLetters++;
    }
    if(isRightChar(c)){

        for(i=0;i<strlen(currAns);i++){
            if(currAns[i]==c){
                encrAns[i]=c;
            }
        }
        gtk_label_set_text((GtkLabel *)g_lbl_answer,(const gchar *)encrAns);
        setStage();
    }
    else{
        gameStage++;
        setStage();
    }
}



int isValidText(char text[])
{
    int n=strlen(text);
    if(n!=1){
        return 0;
    }
    else if(!(isalpha(text[0]))){
        return 0;
    }
    else
        return 1;
}

void genAlert(char message[])
{   
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new(GTK_WINDOW(window), 
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_INFO, 
                                  GTK_BUTTONS_OK,
                                  message );
    gtk_window_set_title(GTK_WINDOW(dialog), "Alert!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
}

char *strupr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = toupper((unsigned char)*p);
      p++;
  }

  return str;
}



void on_btn_start_clicked()
{
    dispHint();
    dispAnswer();
    gameStage=0;
    setStage();
    for(int i=0;i<50;i++){
        enteredLetters[i]='\0';
    }    showGame();
    return;
}

void on_btn_main_menu_clicked()
{
    gtk_window_close(GTK_WINDOW(window));
    quitGame=1;
    return;
}

void on_btn_rehint_clicked()
{
    rehinted=1;
    numLetters=0;
    for(int i=0;i<50;i++){
        enteredLetters[i]='\0';
    }
    strcpy(encrAns,"");
    dispHint();
    dispAnswer();
    gameStage=0;
    setStage();
    return;
}

void on_btn_submit_clicked()
{   
    char message1[]="You can only enter alphabets (one at a time)!";
    char ip_text[20];
    strcpy(ip_text,gtk_entry_get_text(GTK_ENTRY(g_txt_letter)));
    gtk_entry_set_text(GTK_ENTRY(g_txt_letter),"");
    gtk_widget_grab_focus(g_txt_letter);
    if (isValidText(ip_text)){
        processChar(ip_text[0]);
        return;
    }
    else{
        genAlert(message1);
        return;
    }
} 

void on_btn_quit_clicked()
{   
    int quit=confirmExit();
    if (quit)
        gtk_main_quit();
    return;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}