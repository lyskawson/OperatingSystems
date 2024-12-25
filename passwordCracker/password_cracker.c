#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <ctype.h>


// gcc -o cracker password_cracker.c -lm -lpthread -lssl -lcrypto 

//gcc -o cracker password_cracker.c -I/opt/homebrew/Cellar/openssl@3/3.4.0/include -L/opt/homebrew/Cellar/openssl@3/3.4.0/lib -lm -lpthread -lssl -lcrypto



#define MAX_HASHES 1000      
#define MAX_HASH_LENGHT 33  // hash lehnght (32) + terminator '\0' (1)
#define MAX_EMAIL_LENGHT 50        




typedef struct {
    int id;
    char hash[MAX_HASH_LENGHT];
    char email[MAX_EMAIL_LENGHT];
    int cracked;
    char cracked_password[256]; 
} HASH;


void bytes2md5(const char *data, int len, char *md5buf) 
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, data, len);
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);
    for (i = 0; i < md_len; i++) {
        snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
    }
}


int load_dictionary(const char *filename, char ***dictionary) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(1);
    }

    *dictionary = NULL;
    int word_count = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        buffer[strcspn(buffer, "\r\n")] = '\0'; // remove newline 

        *dictionary = realloc(*dictionary, (word_count + 1) * sizeof(char *));// resize the dictionary array
        (*dictionary)[word_count] = malloc(strlen(buffer) + 1); // allocate memory for the new word
        
        strcpy((*dictionary)[word_count], buffer); // copy the word to the dictionary
        word_count++;
    }

    fclose(file);
    return word_count;
}



int load_hashes(const char *hashes_file, HASH *hashes) 
{
    FILE *file = fopen(hashes_file, "r");
    if (!file) 
    {
        fprintf(stderr, "Error: Unable to open file %s\n", hashes_file);
        exit(1);
    }

    

    char *buffer = NULL;
    size_t buffer_size = 0;
    int hash_count = 0;

    while (getline(&buffer, &buffer_size, file) != -1) 
    {
        buffer[strcspn(buffer, "\r\n")] = '\0'; // Remove newline

        char *token;
        // Parsing line [id] [hashed password] [email] [username]
        token = strtok(buffer, "\t ");
        if (token) 
        {
            hashes[hash_count].id = atoi(token); // Save id
        }

        token = strtok(NULL, "\t "); // Hashed password
        if (token) 
        {
            strncpy(hashes[hash_count].hash, token, sizeof(hashes[hash_count].hash));
        }

        token = strtok(NULL, "\t "); // Email
        if (token) 
        {
            strncpy(hashes[hash_count].email, token, sizeof(hashes[hash_count].email));
        }

        hash_count++;
    }

    free(buffer);
    fclose(file);
    return hash_count;
}


void to_uppercase(char *word) 
{
    for (int i = 0; word[i]; i++) 
    {
        word[i] = toupper(word[i]);
    }
}

void to_lowercase(char *word) 
{
    for (int i = 0; word[i]; i++) 
    {
        word[i] = tolower(word[i]);
    }
}

void capitalize(char *word) 
{
    if (word[0]) 
    {
        word[0] = toupper(word[0]);
    }
    for (int i = 1; word[i]; i++) 
    {
        word[i] = tolower(word[i]);
    }
}

char **generate_variants(char *word, int *variant_count) 
{
    char prefix[3];
    char postfix[3];
    int capacity = 1000;  

    char **variants = malloc(capacity * sizeof(char *));
    *variant_count = 0;

    for (int i = 0; i <= 99; i++) 
    {
        snprintf(prefix, sizeof(prefix), "%d", i);
        for (int j = 0; j <= 99; j++) 
        {
            snprintf(postfix, sizeof(postfix), "%d", j);

         
            char lowercase[256], uppercase[256], capitalized[256];
            strcpy(lowercase, word);
            strcpy(uppercase, word);
            strcpy(capitalized, word);

            to_lowercase(lowercase);
            to_uppercase(uppercase);
            capitalize(capitalized);

            char *cases[] = { word, lowercase, uppercase, capitalized };
            int case_count = 4;

            for (int k = 0; k < case_count; k++) 
            {
                // original
                if (*variant_count >= capacity) 
                {
                    capacity *= 2;
                    variants = realloc(variants, capacity * sizeof(char *));
                }
                variants[*variant_count] = malloc(strlen(cases[k]) + 1); //space for word + terminator
                strcpy(variants[*variant_count], cases[k]);
                (*variant_count)++;

                // prefix + word
                if (*variant_count >= capacity) 
                {
                    capacity *= 2;
                    variants = realloc(variants, capacity * sizeof(char *));
                }
                variants[*variant_count] = malloc(strlen(prefix) + strlen(cases[k]) + 1); //space for prefix + word + terminator
                snprintf(variants[*variant_count], 256, "%s%s", prefix, cases[k]);
                (*variant_count)++;

                // word + postfix
                if (*variant_count >= capacity) 
                {
                    capacity *= 2;
                    variants = realloc(variants, capacity * sizeof(char *));
                }
                variants[*variant_count] = malloc(strlen(cases[k]) + strlen(postfix) + 1); //space for word + postfix + terminator
                snprintf(variants[*variant_count], 256, "%s%s", cases[k], postfix);
                (*variant_count)++;

                // prefix + word + postfix
                if (*variant_count >= capacity) 
                {
                    capacity *= 2;
                    variants = realloc(variants, capacity * sizeof(char *));
                }
                variants[*variant_count] = malloc(strlen(prefix) + strlen(cases[k]) + strlen(postfix) + 1); //space for prefix + word + postfix + terminator
                snprintf(variants[*variant_count], 256, "%s%s%s", prefix, cases[k], postfix);
                (*variant_count)++;
            }
        }
    }

    return variants;
}

void display_cracked_passwords(HASH *hashes, int hashes_size) 
{
    printf("\n------------------ CRACKED PASSWORDS ------------------\n");
    for (int i = 0; i < hashes_size; i++) 
    {
        if (hashes[i].cracked) 
        {
            printf("Password for %s is %s\n", hashes[i].email, hashes[i].cracked_password);
        }
    }
    printf("------------------------------------------------------- \n");
}


void *crack_passwords(void *args) 
{
  
    char **dictionary = ((void **)args)[0];
    HASH *hashes = ((void **)args)[1];
    int dictionary_size = *(int *)((void **)args)[2];
    int hashes_size = *(int *)((void **)args)[3];

    for (int i = 0; i < dictionary_size; i++) 
    {
        char md5[33];
        int variants_count = 0;

        char **variants = generate_variants(dictionary[i], &variants_count);

        for (int j = 0; j < variants_count; j++) 
        {
            bytes2md5(variants[j], strlen(variants[j]), md5);
            for (int k = 0; k < hashes_size; k++) 
            {
                if (!hashes[k].cracked && strcmp(md5, hashes[k].hash) == 0) 
                {
                    hashes[k].cracked = 1;
                    strncpy(hashes[k].cracked_password, variants[j], sizeof(hashes[k].cracked_password)); // Zapisanie złamanego hasła
                    //printf("Cracked: %s\n", hashes[k].email);
                }
            }
            free(variants[j]);
        }
        free(variants);
    }
    return NULL;
}

void check_single_email(HASH *hashes, int hashes_size, char **dictionary, int dictionary_size) 
{
    char email[MAX_EMAIL_LENGHT];
    printf("\nEnter e-mail address: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\r\n")] = '\0'; 
    
    // search for hash
    int hash_index = -1;
    for (int i = 0; i < hashes_size; i++) 
    {
        if (strcmp(hashes[i].email, email) == 0) 
        {
            hash_index = i;
            break;
        }
    }

    if (hash_index == -1) 
    {
        printf("E-mail not found.\n");
        return;
    }

    //checking hashes
    printf("Checking hashes...\n");
    char md5[33];
    for (int i = 0; i < dictionary_size; i++) 
    {
        int variants_count = 0;
        char **variants = generate_variants(dictionary[i], &variants_count);

        for (int j = 0; j < variants_count; j++) 
        {
            bytes2md5(variants[j], strlen(variants[j]), md5);

            if (strcmp(md5, hashes[hash_index].hash) == 0) 
            {
                printf("Password for %s is %s\n", email, variants[j]);
                hashes[hash_index].cracked = 1;
                strncpy(hashes[hash_index].cracked_password, variants[j], sizeof(hashes[hash_index].cracked_password));
                for (int k = 0; k < variants_count; k++) 
                {
                    free(variants[k]);
                }
                free(variants);
                return;
            }
        }

        for (int k = 0; k < variants_count; k++)   
        {
            free(variants[k]);
        }
        free(variants);
    }

    printf("Password for %s not found\n", email);
}


int main() {
    char **dictionary; // array of words from dictionary
    HASH hashes[MAX_HASHES]; // array of hashes

    int dictionary_size = load_dictionary("test-dict-large.txt", &dictionary);
    int hashes_size = load_hashes("test-data5.txt", hashes);

    pthread_t crack_thread;  // thread for cracking passwords 

    void *args[4] = {dictionary, hashes, &dictionary_size, &hashes_size}; //arguments for crack_passwords
    
    pthread_create(&crack_thread, NULL, crack_passwords, args);
    printf("\nStart cracking passwords...\n");

    // Menu
    char input;
    while (1) {
        printf("\nMenu:\n");
        printf("p - Dipslay cracked passwords\n");
        printf("l - Check e-mail\n");
        printf("q - Exit\n");
        printf("Choose option: ");
        input = getchar();
        while (getchar() != '\n'); // clear buffer

        if (input == 'p') 
        {
            display_cracked_passwords(hashes, hashes_size);
        } 
        else if (input == 'q') 
        {
            printf("Exit program...\n");
            
            //pthread_join(crack_thread, NULL);  // wait for thread to finish
            break;
        }
        else if (input == 'l') 
        {
            check_single_email(hashes, hashes_size, dictionary, dictionary_size); 
        }
        else 
        {
            printf("Invalid operation.\n");
        }
    }

    
    for (int i = 0; i < dictionary_size; i++) 
    {
        free(dictionary[i]);
    }
    free(dictionary);

    return 0;
}