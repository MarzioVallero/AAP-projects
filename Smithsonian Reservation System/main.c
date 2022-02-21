/** Vallero Marzio   s249920
 *  Algorithms and Programming Exam
 *  Part II: Program (18 point version)
 */

///<ctype.h>, <string.h> and "#define MXLEN 256" were included
///in the library "treePublic.h". I've pasted all functions from
///external libraries here for ease of reading.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MXLEN 256

///Struct used to create the BST for the visitors
///and to store their reservations.
typedef struct visitor_t{
    char *name, *museum_name;
    int hour1, min1, hour2, min2, day, n_people;
    struct visitor_t *left, *right, *next;
} visitor_t;
///Struct used to store the information regarding a
///specific time slot for a museum. Successive time
///slots are enlisted after the first.
typedef struct time{
    int hour1, min1, hour2, min2, threshold;
    struct time *next;
} time_type;
///Struct used to create an array of days (calendar)
///and to keep track of the actual day (even though it's
///not necessary, since it's also the index of the
///array of struct "calendar").
typedef struct days_t{
    int date;
    struct time *timeslotshead;
} days_t;
///This struct holds the museum name and the "calendar".
typedef struct museum{
    char *name;
    struct days_t **calendar;
} museum_t;
///Wrapper added in order to manipulate the museumBST.
///I did not include this struct int the written exam,
///I added it in order to not modify the original struct museum,
///even though I planned to have pointers to left and right
///within struct museum, as it is with struct visitor_t.
typedef struct wrapper_m_t {
    museum_t *museum;
    struct wrapper_m_t *left, *right;
} wrapper_m_t;

///All the Prototypes for the functions used.
///Most of those come from the libraries that I
///have included in the written exam.
visitor_t *createEmptyTree (void);
int format_input(char *);
wrapper_m_t *load_system(char *);
wrapper_m_t * insert_museum (wrapper_m_t *, char *, int);
void insert_timeslot(wrapper_m_t*, int, int, int, int, int, int);
time_type *initialize_time_slots(int);
wrapper_m_t *search_museum(wrapper_m_t *, char *);
visitor_t *search_visitor(visitor_t *, char *);
void print_reservations(visitor_t *, char *);
visitor_t *insert_visitor_r(visitor_t *, char *, int, int, int, int, int, int, char *);
void insert_visitor(wrapper_m_t *, visitor_t **, char *, int, int, int, int, int, int, char *);
int add_reservation(wrapper_m_t *, visitor_t **, char *, int, int, int, int, int, int, char *);
void museumsBST_dispose(wrapper_m_t *);
void visitorsBST_dispose(visitor_t *);
void museum_struct_dispose(museum_t *);
void days_struct_dispose(days_t **);
void time_struct_dispose(time_type *);

///The filename is obtained from the program's arguments.
///In the program's folder a "source.txt" file is included for this scope.
int main(int argc, char **argv)
{
    ///Check for the input argument.
    if(argc!=2){
        fprintf(stdout, "Error. Filename missing.\n");
        exit(EXIT_FAILURE);
    }
    ///Initialization of the main variables used by the program.
    char buffer[MXLEN], m_name[MXLEN], res_name[MXLEN];
    int hour1, min1, hour2, min2, day, n_people;
    ///VisitorsBST is initialized at NULL, since it's empty.
    visitor_t *visitorsBST = createEmptyTree();
    ///The museumsBST is initialized in an external function
    ///that returns the pointer to the Tree.
    wrapper_m_t *museumsBST = load_system(argv[1]);

    ///Variable used to keep the menu up, or close it.
    int flag = 1;
    while(flag){
        fflush(stdin); ///Reset stdin to avoid reservation chaining.
        memset(buffer, 0, MXLEN); ///Reset buffer for safety.
        fprintf(stdout, "\nPlease, enter a command.\nAvailable commands are \"NewReservation\" , \"ShowReservations\" and \"Close\".\n");
        fgets(buffer, MXLEN, stdin);
        ///The switch checks only for the first character,
        ///since all commands have a different starting letter.
        switch(buffer[0]){
        case 'N':
            memset(buffer, 0, MXLEN); ///Reset buffer for safety.
            fprintf(stdout, "Please, enter your reservation here:\n");
            fgets(buffer, MXLEN, stdin);
            ///The input is "cleaned" of all punctuation characters except "_"
            format_input(buffer);
            sscanf(buffer, "%s %d %d %d %d %d %d %s", m_name, &day, &hour1, &min1,
                   &hour2, &min2, &n_people, res_name);
            ///add_reservation returns 1 on success or 0 if it encounters any error.
            if(add_reservation(museumsBST, &visitorsBST, m_name, day, hour1, min1,
                               hour2, min2, n_people, res_name) == 1){
                fprintf(stdout, "Your reservation has been processed.\n");
            } else {
                fprintf(stdout, "Sorry, we couldn't place your reservation.\n");
            }
            break;
        case 'S':
            memset(buffer, 0, MXLEN); ///Reset buffer for safety.
            fprintf(stdout, "Please, enter the name you wish to check reservations for:\n");
            fscanf(stdin, "%s", res_name);
            print_reservations(visitorsBST, res_name);
            break;
        case 'C':
            memset(buffer, 0, MXLEN); ///Reset buffer for safety.
            fprintf(stdout, "Thanks for using the Smithsonian Reservation System.\n");
            flag=0; ///Exit condition.
            break;
        default:
            fprintf(stdout, "Unknown command.\n");
            break;
        }
    }
    ///Free all memory allocated for the data structures.
    museumsBST_dispose(museumsBST);
    visitorsBST_dispose(visitorsBST);

    return EXIT_SUCCESS;
}

visitor_t *createEmptyTree (void){
  return (NULL);
}

int format_input(char *string){
    int i = 0;
    while(string[i]!='\0'){
        if(ispunct(string[i])!=0 && string[i]!='_'){
            string[i]=' ';
        }
        ++i;
    }
    return 0;
}

///Time slots are initialized at -1, so I can check whether
///I have already inserted one in that position or if I have
///to go further down the list of time slots.
time_type *initialize_time_slots(int n){
    time_type *tmptr = NULL;
    if(n == 0){
        return (NULL);
    } else {
        tmptr = malloc(sizeof(time_type));
        tmptr->hour1 = -1;
        tmptr->min1 = -1;
        tmptr->hour2 = -1;
        tmptr->min2 = -1;
        tmptr->threshold = -1;
        tmptr->next = initialize_time_slots(n-1);
    }
    return (tmptr);
}

///Allocation of new museums in the museumsBST.
///Library function modified to accomodate for struct type.
wrapper_m_t * insert_museum (wrapper_m_t *rp, char *name, int n_timeslots){
    wrapper_m_t *p;
    int year = 365;

    ///Found an empty child, put in the tree the new museum.
    if (rp == NULL) {
        p = malloc(sizeof(wrapper_m_t));
        p->left = NULL;
        p->right = NULL;
        p->museum = malloc(sizeof(museum_t));
        p->museum->name = strdup(name);
        p->museum->calendar = malloc(year*sizeof(days_t *));
        for(int i=0; i<year; ++i){
            p->museum->calendar[i] = malloc(sizeof(days_t));
            p->museum->calendar[i]->date = i;
            p->museum->calendar[i]->timeslotshead = initialize_time_slots(n_timeslots);
        }
        p->left = p->right = NULL;
        return (p);
    }

    ///Case of duplicated museum, nothing to be done.
    if (strcmp(name, rp->museum->name) == 0) {
        return (rp);
    }

    if (strcmp(name, rp->museum->name) < 0) {
        ///Insert on the left.
        rp->left = insert_museum (rp->left, name, n_timeslots);
    } else {
        ///Insert on the right.
        rp->right = insert_museum (rp->right, name, n_timeslots);
    }

    return (rp);
}

///Insert timeslots on their respective museum once
///they are read from the file. Successive time slots
///of the same museum are enqueued.
void insert_timeslot(wrapper_m_t* rp, int hour1, int min1, int hour2, int min2, int threshold, int n_time_slots){
    int year = 365;
    for(int i=0; i<year; ++i){
        time_type *tmptr = rp->museum->calendar[i]->timeslotshead;
        for(int j=0; j<n_time_slots && tmptr != NULL; ++j){
            if(tmptr->threshold == -1){
                tmptr->hour1 = hour1;
                tmptr->min1 = min1;
                tmptr->hour2 = hour2;
                tmptr->min2 = min2;
                tmptr->threshold = threshold;
                j=n_time_slots+2;
            } else {
                tmptr = tmptr->next;
            }
        }
    }
    return;
}

///Function that creates the pointer to the museumBST
///to be returned to main.
wrapper_m_t *load_system(char *filename){
    int n_time_slots;
    char buffer[MXLEN];
    wrapper_m_t *root = NULL, *current = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stdout, "Error, couldn't allocate memory for the system.\n");
    }
    ///Read from the file and decide what to do with it.
    while(fgets(buffer, MXLEN-1, fp)!=NULL){
        ///If the first character is not a digit, it means we
        ///read a museum name and relative n_time_slots.
        ///Puts the museum in the Tree.
        if(isdigit(buffer[0]) == 0){
            char m_name[MXLEN];
            sscanf(buffer, "%s %d", m_name, &n_time_slots);
            root = insert_museum(root, m_name, n_time_slots);
            current = search_museum(root, m_name);
        }
        ///Otherwise, we read a time slot. Puts the time slot
        ///in the correct museum, that is, the most recent museum
        ///inserted in the Tree (i.e. "current").
        else {
            int hour1, min1, hour2, min2, threshold;
            format_input(buffer);
            sscanf(buffer, "%d %d %d %d %d", &hour1, &min1, &hour2, &min2, &threshold);
            insert_timeslot(current, hour1, min1, hour2, min2, threshold, n_time_slots);
        }
        memset(buffer, 0, MXLEN);
    }
    fclose(fp);
    return root;
}

///Search for a museum pointer and return it to the calling function.
///Library function modified to accomodate for struct type.
wrapper_m_t *search_museum(wrapper_m_t *rp, char *name){
    ///Error. Museum not found.
    if (rp == NULL) {
        return (rp);
    }

    ///Museum found!
    if (strcmp(name, rp->museum->name) == 0) {
        return (rp);
    }

    if(strcmp(name, rp->museum->name) < 0) {
        ///Search on the left.
        return (search_museum (rp->left, name));
    } else {
        ///Search on the right.
        return (search_museum (rp->right, name));
    }

    return (rp);
}

///Function that checks for the data to not be malformed.
///If it is correctly formatted, a reservation is inserted in visitorsBST, returning 1.
///Otherwise, a respective error is printed on stdout, returning 0.
///This is different from the written exam, as I felt it would have been easier to
///test the program if it didn't hard-close every time it recieves malformed data.
///Functionally, the code is the same.
int add_reservation(wrapper_m_t *museumsBST, visitor_t **visitorsBST, char *m_name,
                     int day, int hour1, int min1, int hour2, int min2, int n_people, char *res_name)
{
    int flag = 0;
    wrapper_m_t * tmp = NULL;
    tmp = search_museum(museumsBST, m_name);
    if(tmp == NULL){
        fprintf(stdout, "Error, museum not found.\n");
        return 0;
    }
    ///Thanks to the format_input() function, if a day with a "-" sign is inserted,
    ///the "-" sign is removed, and the day is considered in "absolute value".
    if(day<0 || day > 364){
        fprintf(stdout, "Error, invalid date.\n");
        return 0;
    }
    time_type *checktmp = NULL;
    checktmp = tmp->museum->calendar[day]->timeslotshead;
    ///Check if the time slot inserted corresponds to any of the available
    ///time slot for the requested museum. If none correspond, an error is
    ///printed and the function returns 0.
    while(checktmp!=NULL){
        if(checktmp->hour1 == hour1 && checktmp->min1 == min1 &&
           checktmp->hour2 == hour2 && checktmp->min2 == min2){
            flag = 1;
            break;
        } else {
            checktmp = checktmp->next;
        }
    }
    if(flag == 0){
        fprintf(stdout, "Error, invalid time interval.\n");
        return 0;
    }
    ///If the requested tickets are available, the reservation is inserted.
    ///The museum's threshold for that day is updated in the function insert_visitor().
    if(checktmp->threshold >= n_people){
        insert_visitor(museumsBST, &(*visitorsBST), res_name, n_people, day, hour1, min1, hour2, min2, m_name);
    } else {
        fprintf(stdout, "Error. Not enough tickets left for the requested day and time slot.\n");
        return 0;
    }
    return 1;
}

///Wrapper function used to update the museum's threshold in the reservation
///day and to call for the recursive insertion in the visitorsBST.
///The museumBST argument has been added as I forgot to put in the written exam.
///Library function modified to accomodate for struct type.
void insert_visitor(wrapper_m_t *museumsBST, visitor_t **rp, char *res_name,
                          int n_people, int day, int hour1, int min1, int hour2, int min2, char *m_name){
    wrapper_m_t * tmp_wrap = NULL;
    tmp_wrap = search_museum(museumsBST, m_name);
    time_type *checktmp = NULL;
    checktmp = tmp_wrap->museum->calendar[day]->timeslotshead;
    checktmp->threshold = (checktmp->threshold) - n_people;
    *rp = insert_visitor_r(*rp, res_name, n_people, day, hour1, min1, hour2, min2, m_name);
    return;
}

///Recursive search in the visitorsBST for the correct position and either insert new visitor
///or enqueue another reservation after previous ones from the same person.
///Library function modified to accomodate for struct type.
visitor_t *insert_visitor_r(visitor_t *rp, char *res_name, int n_people,
                            int day, int hour1, int min1, int hour2, int min2, char *m_name){
    visitor_t *tmp = NULL;

    ///Empty position: a new reservation is placed.
    if (rp == NULL) {
        tmp = malloc(sizeof(visitor_t));
        tmp->name = strdup(res_name);
        tmp->n_people = n_people;
        tmp->day = day;
        tmp->hour1 = hour1;
        tmp->min1 = min1;
        tmp->hour2 = hour2;
        tmp->min2 = min2;
        tmp->museum_name = strdup(m_name);
        tmp->left = NULL;
        tmp->right = NULL;
        tmp->next = NULL;
        return (tmp);
    }

    ///Previous  reservation present, enqueue after that one.
    if (strcmp(res_name, rp->name) == 0) {
        tmp = rp;
        while(tmp->next!=NULL){
            tmp = tmp->next;
        }
        tmp->next = malloc(sizeof(visitor_t));
        tmp->next->name = strdup(res_name);
        tmp->next->n_people = n_people;
        tmp->next->day = day;
        tmp->next->hour1 = hour1;
        tmp->next->min1 = min1;
        tmp->next->hour2 = hour2;
        tmp->next->min2 = min2;
        tmp->next->museum_name = strdup(m_name);
        tmp->next->left = NULL;
        tmp->next->right = NULL;
        tmp->next->next = NULL;
        return (rp);
    }

    if (strcmp(res_name, rp->name) < 0) {
        ///Recur to insert on the left.
        rp->left = insert_visitor_r(rp->left, res_name, n_people, day, hour1, min1, hour2, min2, m_name);
    } else {
        ///Recur to insert on the right.
        rp->right = insert_visitor_r(rp->right, res_name, n_people, day, hour1, min1, hour2, min2, m_name);
    }

    return (rp);
}

///Recursive search for a visitor's position in the visitorsBST.
///Library function modified to accomodate for struct type.
///Changed from the one used in the written exam,but identical in use.
visitor_t *search_visitor(visitor_t *rp, char *name){
    ///Visitor not found, return a NULL pointer.
    if (rp == NULL) {
        return (rp);
    }

    ///Visitor found, return its pointer.
    if (strcmp(name, rp->name) == 0) {
        return (rp);
    }

    if(strcmp(name, rp->name) < 0) {
        ///Recursive search on the left.
        return (search_visitor(rp->left, name));
    } else {
        ///Recursive search on the right.
        return (search_visitor(rp->right, name));
    }

    return (rp);
}

///Function that searches for the given name in the visitorsBST.
///If the person is found, all its reservations are printed on stdout,
///otherwise an error is printed on stdout.
void print_reservations(visitor_t *rp, char *res_name){
    visitor_t *tmp = NULL;
    tmp = search_visitor(rp, res_name);
    if(tmp != NULL){
        fprintf(stdout, "Here's the list of all reservations made under the name %s:\n", res_name);
        while(tmp != NULL){
            fprintf(stdout, "At %s, on day %d, in the time slot %d.%02d-%d.%02d, for %d people.\n",
                    tmp->museum_name, tmp->day, tmp->hour1, tmp->min1, tmp->hour2, tmp->min2, tmp->n_people);
            tmp = tmp->next;
        }
    } else {
        fprintf(stdout, "No reservations under the name \"%s\" are present in our system.\n", res_name);
    }
    return;
}

///Function that frees the memory associated to the museumsBST.
///Recursively frees its childs and calls the functions created
///to free the other structs allocated within it.
void museumsBST_dispose(wrapper_m_t *rp){
    if (rp == NULL) {
        return;
    }

    museumsBST_dispose(rp->left);
    museumsBST_dispose(rp->right);
    museum_struct_dispose(rp->museum);
    free(rp);

    return;
}

///Call to free the calendar.
void museum_struct_dispose(museum_t *rp){
    days_struct_dispose(rp->calendar);
    free(rp);
}

///Call to free the time slots associated to the museum.
void days_struct_dispose(days_t **rp){
    for(int i=0; i<364; i++){
        time_struct_dispose(rp[i]->timeslotshead);
    }
    free(rp);
}

///Free the list of time slots.
void time_struct_dispose(time_type *rp){
    time_type *tmp = NULL;
    while(rp != NULL){
        tmp = rp;
        rp = rp->next;
        free(tmp);
    }
}

///Function that frees the memory associated to the visitorsBST.
void visitorsBST_dispose(visitor_t *rp){
    if (rp == NULL) {
        return;
    }

    visitorsBST_dispose(rp->left);
    visitorsBST_dispose(rp->right);
    visitor_t *tmp = NULL;
    while(rp != NULL){
        tmp = rp;
        rp = rp->next;
        free(tmp);
    }

    return;
}

