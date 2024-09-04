#include "vector.c"

//generate kmp table of pattern (used only in write_pattern_file.c)
void gen_kmp_table(int_vector cv, int_vector *kmp_table){

    int pos = 1;
    int cnd = 0;

    pushback_int(kmp_table, -1);

    while(pos < cv.size){
        if(cv.Array[pos] == cv.Array[cnd])
            pushback_int(kmp_table, (*kmp_table).Array[cnd]);
        else{
            pushback_int(kmp_table, cnd);
            while ((cnd >= 0) && (cv.Array[pos] != cv.Array[cnd]))
            {
                cnd = (*kmp_table).Array[cnd];
            }
        }
        pos++;
        cnd++;
    }
    pushback_int(kmp_table, cnd);
}

//save patterns with their kmp table
int main(){
    
    FILE *fp;
    int data, data_len;
    int_vector pat = {NULL,0}, kmp_table = {NULL,0};

    scanf("%d", &data_len);

    fp = fopen("patterns.txt", "a");
    fprintf(fp,"%d ", data_len);
    for(int i = 0; i < data_len; i++){
        scanf("%d", &data);
        fprintf(fp, "%d ", data);
        pushback_int(&pat, data);
    }

    gen_kmp_table(pat, &kmp_table);

    for(int i = 0; i < data_len; i++) fprintf(fp, "%d ", kmp_table.Array[i]);

    scanf("%d", &data);
    fprintf(fp, "%d\n", data);


    fclose(fp);
}