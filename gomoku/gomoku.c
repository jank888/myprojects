#include "vector.c"

#define INF 100000000

//save board
int save_board(grid board, point_vector possible_placements){
    //open or create file
    FILE *fp;
    int status;

    fp = fopen("saved_board.txt", "w");
    if(fp == NULL){
        return 1;
    }
    //save board size
    fprintf(fp,"%d %d ", board.n, board.m);

    //save board state
    for(int i = 0; i < board.values.size; i++) fprintf(fp, "%d ", board.values.Array[i]);
    fprintf(fp, "%d ", possible_placements.size);
    for(int i = 0; i < possible_placements.size; i++) fprintf(fp, "%d %d ", possible_placements.Array[i].x, possible_placements.Array[i].y);

    //close file
    status = fclose(fp);
    if(status != 0) return 1;
    return 0;
}

//load board from file
void load_board(grid *board, point_vector *possible_placements){

    //init, open file
    int data1, data2;
    point pos;
    FILE *fp = fopen("saved_board.txt", "r");

    //load size of board
    fscanf(fp, "%d %d ", &data1, &data2);
    board->n = data1;
    board->m = data2;

    //load state of board
    for(int i = 0; i < board->n*board->m; i++){
        fscanf(fp, "%d ", &data1);
        pushback_int(&board->values, data1);
    }

    //load possible_placements
    fscanf(fp, "%d ", &data1);
    for(int i = 0; i < data1; i++){
        fscanf(fp, "%d %d ", &pos.x, &pos.y);
        pushback_point(possible_placements, pos);
    }

    fclose(fp);
}

//load patterns to search for
void load_patterns(pattern patterns[N]){

    //init, open file
    int data, len_data;
    FILE *fp = fopen("patterns.txt", "r");

    //load patterns
    for(int i = 0; i < N; i++){
        //load the pattern
        fscanf(fp, "%d ", &len_data);
        for(int j = 0; j < len_data; j++){
            fscanf(fp, "%d ", &data);
            pushback_int(&patterns[i].int_pattern, data);
        }
        //load patterns kmp_table
        for(int j = 0; j < len_data; j++){
            fscanf(fp, "%d ", &data);
            pushback_int(&patterns[i].kmp_table, data);
        }
        fscanf(fp, "%d", &data);
        patterns[i].point_value = data;
    }
    fclose(fp);

}
//print grid
void print_grid(grid board){

    for(int i = 0; i < board.m; i++){
            printf("%d ",(i+1)%10);
            for(int j = 0; j < board.n; j++){
                if(board.values.Array[i*board.n+j] == -1) printf("X ");
                if(board.values.Array[i*board.n+j] == 0) printf("- ");
                if(board.values.Array[i*board.n+j] == 1) printf("O ");
            }
            printf("\n");
        }
        printf("  ");
        for(int i = 0; i < board.n; i++){
            printf("%d ",(i+1)%10);
        }
        printf("\n");
}

//search algorithm with help of kmp_table
int search_pattern(pattern pat,grid board){

    int matches = 0;

    //search for pattern in rows
    for(int i = 0; i < board.m; i++){
        int k = 0;
        int j = 0;

        while(j < board.n){
            if(pat.int_pattern.Array[k] == board.values.Array[i*board.n+j]){
                k++;
                j++;
                if(k == pat.int_pattern.size){
                    matches++;
                    k = 0;
                }
            }
            else{
                k = pat.kmp_table.Array[k];
                if(k < 0){
                    j++;
                    k++;
                }
            }
        }
    }
    //search for pattern in columns
    for(int i = 0; i < board.n; i++){
        int k = 0;
        int j = 0;
        while(j < board.m){
            if(pat.int_pattern.Array[k] == board.values.Array[j*board.n+i]){
                k++;
                j++;
                if(k == pat.int_pattern.size){
                    matches++;
                    k = 0;
                }
            }
            else{
                k = pat.kmp_table.Array[k];
                if(k < 0){
                    j++;
                    k++;
                }
            }
        }
    }
    //search for patterns in diagonal 1s
    for(int l = 0; l<board.n+board.m-1; l++){
        int k = 0;
        int j = 0;
        while(j <= l){
            int i = l - j;
            if((i < board.m) && (j < board.n)){
                if(pat.int_pattern.Array[k] == board.values.Array[i*board.n+j]){
                    k++;
                    j++;
                    if(k == pat.int_pattern.size){
                        matches++;
                        k = 0;
                    }
                }
                else{
                    k = pat.kmp_table.Array[k];
                    if(k < 0){
                        j++;
                        k++;
                    }
                }
            }
            else j++;
        }
    } 
    //search for pattern in diagonal2
    for(int l = 0; l<board.n+board.m-1; l++){
        int k = 0;
        int j = 0;
        while(j <= l){
            int i = l - j;
            if((i < board.m) && (j < board.n)){
                if(pat.int_pattern.Array[k] == board.values.Array[i*board.n+(board.n-j-1)]){
                    k++;
                    j++;
                    if(k == pat.int_pattern.size){
                        matches++;
                        k = 0;
                    }
                }
                else{
                    k = pat.kmp_table.Array[k];
                    if(k < 0){
                        j++;
                        k++;
                    }
                }
            }
            else j++;
        }
    } 
    return matches;
}

//check if somebody won
int check_game_over(grid board, pattern patterns[N]){
    //return 1 if AI wins, return, 0 if players win, return 0 if nobody is winning
    if(search_pattern(patterns[0],board) != 0) return 1;
    if(search_pattern(patterns[1],board) != 0) return -1;
    return 0;
}

//check if place is available on board
int available(point pos, grid board){
    if(board.values.Array[pos.y*board.n+pos.x] == 0)   
        return 1;
    return 0;
}

//update dynamic array of places which aren't taken yet
void updatePossiblePlacements(point_vector *possible_placements, point new_pos, grid board){
    point pos_pos;

    //erase newly placed from array
    erase_point_atindex(possible_placements, find_point(*possible_placements, new_pos));

    //check neighbours of new position
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if((new_pos.x + i-1 >= 0) && (new_pos.y + j-1 >= 0) && (new_pos.x + i-1 < board.n) && (new_pos.y + j-1 < board.m)){
                pos_pos.x = new_pos.x + i-1;
                pos_pos.y = new_pos.y + j-1;
                //if neighbourh is available, pushback into possible_placements
                if(available(pos_pos, board) == 1){
                    if(find_point(*possible_placements,pos_pos) == -1)
                        pushback_point(possible_placements,pos_pos);
                }
            }
        }
    }
}

//heuristic evaluation of board
int evaluate_position(grid board, pattern patterns[N]){

    //check if there is a winning formation
    if(check_game_over(board, patterns) == 1)return INF;
    if(check_game_over(board, patterns) == -1) return -INF;

    //if not then evaluate by searching for patterns and summing their value
    int eval = 0;
    for(int i = 0; i < N; i++) eval += search_pattern(patterns[i], board) * patterns[i].point_value;
    return eval;
}

//place stone on board
void place(grid *board, point pos, int item){
    board->values.Array[pos.y*(board->n)+pos.x] = item;
}

//min max algorithm searches for optimal placements with the help of alpha beta pruning (recursive, returns evaluation)
int min_max(grid board, point_vector possible_placements, int depth, int max_depth, int *best_move, int isMaximizingPlayer, int alpha, int beta, pattern patterns[N]){
    //init eval
    int eval;

    //check if somebody won, if yes return INF, -INF
    if(check_game_over(board, patterns) == 1) return INF;
    if(check_game_over(board, patterns) == -1) return -INF;

    //if max depth is reached return evaluation
    if(depth == max_depth) return evaluate_position(board, patterns);
    
    //if no more spaces left return 0, dont call recall function anymore
    if(possible_placements.size == 0) return 0;
    
    //set evaluation
    if(isMaximizingPlayer == 1) eval = -INF;
    else eval = INF;

    //init new states
    grid new_board = {(int_vector) {NULL, 0}, 0, 0};
    point_vector new_possible_placements = {NULL, 0};
    
    //for every move in possible placements call min_max
    for(int i = 0; i < possible_placements.size; i++){
        
        //create new board
        copy_grid(board, &new_board);
        copy_point_vector(possible_placements, &new_possible_placements);

        //create next state of board
        if(isMaximizingPlayer == 1) place(&new_board, possible_placements.Array[i], 1);            
        else place(&new_board, possible_placements.Array[i], -1);

        //update new possible_placements
        updatePossiblePlacements(&new_possible_placements, possible_placements.Array[i], new_board);
        
        if(isMaximizingPlayer == 1){
            //call min max
            int value = min_max(new_board, new_possible_placements, depth+1, max_depth, best_move, 0, alpha, beta, patterns);
            //update eval and alpha if needed
            if(eval < value){
                //save best move
                if(depth == 1) *best_move = i;
                eval = value;
            }
            if(alpha < eval) alpha = eval;
        }
        else{
            //call min_max
            int value = min_max(new_board, new_possible_placements, depth+1, max_depth, best_move, 1, alpha, beta, patterns);
            //update eval and beta if needed
            if(eval > value) eval = value;
            if(beta > eval) beta = eval;       
        }
        //alpha beta pruning
        if(beta <= alpha) break;          
    }
    return eval;  
}

//function calls min_max to make decision
point ai_place(point_vector possible_placements, grid board, pattern patterns[N]){
    int best_move = 0;
    min_max(board, possible_placements, 1, 5, &best_move, 1, -INF, INF, patterns);
    return possible_placements.Array[best_move];
}

//main function
int main(){

    //initializing variables variables
    int good_input, size_n, size_m;
    char y_n, y_n2;
    point pos;
    point_vector possible_placements = {NULL,0}; 

    //load patterns
    pattern patterns[N];
    for(int i = 0; i < N; i++){
        patterns[i].int_pattern.Array = NULL;
        patterns[i].int_pattern.size = 0;
        patterns[i].kmp_table.Array = NULL;
        patterns[i].kmp_table.size = 0;
        patterns[i].point_value = 0;
    } 
    load_patterns(patterns);

    //load saved position if requested
    printf("Would you like to load saved position(y/n)? ");
    scanf("%c", &y_n);

    grid board = {(int_vector) {NULL,0}, 0,0};

    if(y_n == 'y') load_board(&board, &possible_placements);
    else{
        //filling grid
        printf("Please enter board size(NxN): ");
        scanf("%i %i", &size_n, &size_m);

        board.n = size_n;
        board.m = size_m;
        
        for(int i = 0; i < board.n*board.m; i++){
                pushback_int(&board.values, 0);
        }  

        //select first player
        printf("Would you like to start(y/n)? ");
        scanf("\n%c", &y_n2);

        //if computer goes first, place first stone
        if(y_n2 == 'n'){
            point start_pos = {(int) size_n/2, size_m/2};
            place(&board, start_pos, 1);
    }

    }
  
    while(check_game_over(board, patterns) == 0){
        
        //prin board
        print_grid(board);

        good_input = 0;

        //while user doesnt give a valid input
        while(good_input != 1){

            //scan position where player would like to place a stone
            scanf("%d %d",&pos.x, &pos.y);

            //if input is -1 -1, save the board
            if(pos.x == -1 & pos.y == -1){
                save_board(board, possible_placements);
                break;
            } 
            printf("\n");

            //check if pos is valid
            pos.x--;
            pos.y--;
            good_input = 1;
            if(available(pos, board) != 1){
                printf("It's already occupied\n");
                good_input = 0;
            }
            if((find_point(possible_placements, pos) == -1) && (possible_placements.size != 0)){
                printf("You can only place your stone on places adjacent to an occupied one\n");
                good_input = 0;
            }
        }
        //break if board is saved
        if(pos.x == -1 & pos.y == -1) break; 

        //place stone of player on board and update possible placements
        place(&board, pos, -1);
        updatePossiblePlacements(&possible_placements, pos, board);

        //if player wins or board is full(draw) break
        if (check_game_over(board, patterns) != 0 || possible_placements.size == 0)
            break;

        //computer makes decision, places stone of board, updates possible_placements
        pos = ai_place(possible_placements, board, patterns);
        place(&board, pos, 1);
        updatePossiblePlacements(&possible_placements, pos, board);
        if(possible_placements.size == 0) break;
    }

    print_grid(board);
    

    return 0;
}