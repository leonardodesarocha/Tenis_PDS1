#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>

const float FPS = 60;  

const int SCREEN_W = 512;	//Largura da tela
const int SCREEN_H = 640;	//Altura da tela

ALLEGRO_FONT *FONT_32;

//JOGADOR:
int DIST_FUNDO = 50;		//distancia dos jogadores do fundo da tela
int ALTURA_JOGADOR = 30;	//altura dos jogadores
int LARGURA_JOGADOR = 100;	//largura dos jogadores
float VEL_JOGADOR = 3.5;	//velocidade dos jogadores

//BOLA:
float VEL_MAX_X = 2;		//velocidade horizontal maxima das bolas
float VEL_MIN_X = 1;		//velocidade horizontal minima das bolas
float VEL_MAX_Y = 3;		//velocidade vertical maxima das bolas
float VEL_MIN_Y = 1;		//velocidade vertical minima das bolas

int  TEMP_BOLAS = 2;

//Função para criar números aleatórios
//Usada para criar velocidades e tamanhos diferentes das bolas 
float randfloat(float max, float min) {
	return min + (rand() / (RAND_MAX / ( max - min)));
}

//Criação das Bolas
typedef struct Bola {
	float x, y, r, vel_x, vel_y;
	ALLEGRO_COLOR cor;
	int dentro;
} Bola;

//Gera as 12 bolas
void canhao(Bola bolas[], int i, int posicao) {
	bolas[i].cor = al_map_rgb(200, 200, 200);			//cor da bola
	bolas[i].r = randfloat(4, 8);						//raio da bola
	bolas[i].dentro = 1;								//situacao da bola em relacao ao jogo (1 se esta no jogo / 0 se nao esta no jogo)
	if(posicao == 0)
	{
		bolas[i].x = SCREEN_W - bolas[i].r;						//posicao da bola em relacao a x
		bolas[i].y = bolas[i].r;							//posicao da bola em relacao a y
		bolas[i].vel_x = -randfloat(VEL_MAX_X, VEL_MIN_X);	//velocidade horizontal da bola
		bolas[i].vel_y = randfloat(VEL_MAX_Y, VEL_MIN_Y);	//velocidade vertical da bola
	}
	else if(posicao == 1)
	{
		bolas[i].x = SCREEN_W - bolas[i].r;						//posicao da bola em relacao a x
		bolas[i].y = SCREEN_H - bolas[i].r;							//posicao da bola em relacao a y
		bolas[i].vel_x = -randfloat(VEL_MAX_X, VEL_MIN_X);	//velocidade horizontal da bola
		bolas[i].vel_y = -randfloat(VEL_MAX_Y, VEL_MIN_Y);	//velocidade vertical da bola
	}
	else if(posicao == 2)
	{
		bolas[i].x = bolas[i].r;						//posicao da bola em relacao a x
		bolas[i].y = SCREEN_H - bolas[i].r;							//posicao da bola em relacao a y
		bolas[i].vel_x = randfloat(VEL_MAX_X, VEL_MIN_X);	//velocidade horizontal da bola
		bolas[i].vel_y = -randfloat(VEL_MAX_Y, VEL_MIN_Y);	//velocidade vertical da bola
	}
	else if (posicao == 3)
	{
		bolas[i].x = bolas[i].r;						//posicao da bola em relacao a x
		bolas[i].y = bolas[i].r;							//posicao da bola em relacao a y
		bolas[i].vel_x = randfloat(VEL_MAX_X, VEL_MIN_X);	//velocidade horizontal da bola
		bolas[i].vel_y = randfloat(VEL_MAX_Y, VEL_MIN_Y);	//velocidade vertical da bola
	}
}

//Desenha as bolas que estao no jogo
void desenhaBolas(Bola bolas[]) {
    int i = 0;
	for (int i = 0; i < 20; i++) {
		//so desenhas as bolas que estao dentro do jogo
		if(bolas[i].dentro == 1)
			al_draw_filled_circle(bolas[i].x, bolas[i].y, bolas[i].r, bolas[i].cor);
	}
}


//Criação dos jogadores
typedef struct Jogador {
	float x, y, h, w, vel;
	int dir, esq, cima, baixo, p, pnt;
	ALLEGRO_COLOR cor;
} Jogador;

//Cria Jogador 1 com suas características específicas
void initJogador1(Jogador *p1) {
	p1->h = ALTURA_JOGADOR;					//altura do jogador
	p1->w = LARGURA_JOGADOR;				//largura do jogador
	p1->x = SCREEN_W/2 - p1->w/2;			//posicao do jogador em relacao a x
	p1->dir = 0;							//movimento do jogador para direita (1 se esta em movimento / 0 se nao esta em movimento)
	p1->esq = 0;							//movimento do jogador para esquerda (1 se esta em movimento / 0 se nao esta em movimento)
	p1->cima = 0;							//movimento do jogador para cima (1 se esta em movimento / 0 se nao esta em movimento)
	p1->baixo = 0;							//movimento do jogador para baixo (1 se esta em movimento / 0 se nao esta em movimento)
	p1->vel = VEL_JOGADOR;					//velocidade do jogador
	p1->y = SCREEN_H - DIST_FUNDO - p1->h;	//posicao do jogador em relacao a y
	p1->cor = al_map_rgb(155, 0, 0);		//cor do jogador
	p1->p = 0;								//altura do jogador
	p1->pnt = 0;							//pontos do jogador
}

//Cria Jogador 2 com suas características específicas
void initJogador2(Jogador *p2) {
	p2->h = ALTURA_JOGADOR;					//altura do jogador
	p2->w = LARGURA_JOGADOR;				//largura do jogador
	p2->x = SCREEN_W/2 - p2->w/2;			//posicao do jogador em relacao a x
	p2->dir = 0;							//movimento do jogador para direita (1 se esta em movimento / 0 se nao esta em movimento)
	p2->esq = 0;							//movimento do jogador para esquerda (1 se esta em movimento / 0 se nao esta em movimento)
	p2->cima = 0;							//movimento do jogador para cima (1 se esta em movimento / 0 se nao esta em movimento)
	p2->baixo = 0;							//movimento do jogador para baixo (1 se esta em movimento / 0 se nao esta em movimento)
	p2->vel = VEL_JOGADOR;					//velocidade do jogador
	p2->y = DIST_FUNDO;						//posicao do jogador em relacao a y
	p2->cor = al_map_rgb(0, 0, 155);		//cor do jogador
	p2->p = 0;								//altura do jogador
	p2->pnt = 0;							//pontos do jogador
}

void atualizaJogadores(Jogador *p1, Jogador *p2) {
	// JOGADOR 1
	if (p1->dir && p1->x + p1->vel <= SCREEN_W - p1->w + 1)	//se o Jogador 1 esta se movendo para direita
		p1->x += p1->vel;
	if (p1->esq && p1->x + p1->vel >= 0)					//se o Jogador 1 esta se movendo para erquerda
		p1->x -= p1->vel;

    if (p1->baixo && p1->y + p1->vel <= SCREEN_H - p1->h)	//se o Jogador 1 esta se movendo para baixo
		p1->y += p1->vel;
	if (p1->cima && p1->y + p1->vel >= SCREEN_H/2 + 6)		//se o Jogador 1 esta se movendo para cima
		p1->y -= p1->vel;

	// JOGADOR 2
    if (p2->dir && p2->x + p2->vel <= SCREEN_W - p1->w + 1)	//se o Jogador 2 esta se movendo para direita
		p2->x += p2->vel;
	if (p2->esq && p2->x + p2->vel >= 0)					//se o Jogador 1 esta se movendo para erquerda
		p2->x -= p2->vel;

    if (p2->baixo && p2->y + p2->vel <= SCREEN_H/2 - p1->h)	//se o Jogador 2 esta se movendo para baixo
		p2->y += p2->vel;
	if (p2->cima && p2->y + p2->vel >= 8)					//se o Jogador 1 esta se movendo para cima
		p2->y -= p2->vel;
}

//funcao para desenhar os jogadores
void desenhaJogadores(Jogador p1, Jogador p2) {
	al_draw_filled_rectangle(p1.x, p1.y, p1.x + p1.w, p1.y + p1.h, p1.cor);
	al_draw_filled_rectangle(p2.x, p2.y, p2.x + p2.w, p2.y + p2.h, p2.cor);
}

//funcao para desenhar a quadra
void desenhaQuadra() {
	al_clear_to_color(al_map_rgb(0, 155, 0));
	al_draw_line(0, SCREEN_H/2, SCREEN_W, SCREEN_H/2, al_map_rgb(255,255,255), 3);
}

void escrevePontos() {
    sprintf(pntp1, "%d", p1.pnt);												//escreve a pontuacao do jogador no seu char pertencente
	al_draw_text(size_32, al_map_rgb(150, 0, 0), 10, 10, 0, pntp1);				//escreve o char com a pontuacao do jogador na tela
	sprintf(pntp2, "%d", p2.pnt);												//escreve a pontuacao do jogador no seu char pertencente
	al_draw_text(size_32, al_map_rgb(0, 0, 150), SCREEN_W - 10, 10, 2, pntp2);	//escreve o char com a pontuacao do jogador na tela
}

//funcao para atualizar as bolas que estao no jogo
void atualizaBolas(Bola bolas[], Jogador *p1, Jogador *p2, int bolasjogo) {
	int i = 0;
    for (int i = 0; i < 20; i++)
    {
		if(bolas[i].dentro == 1) 
		{
			bolas[i].x += bolas[i].vel_x;
			bolas[i].y += bolas[i].vel_y;

			//se a bola bate na lateral
			if(bolas[i].x - bolas[i].r<= 0 || bolas[i].x + bolas[i].r >= SCREEN_W)
				bolas[i].vel_x = -bolas[i].vel_x;

			//se a bola bate na parte superior
			if(bolas[i].y + bolas[i].r <= 0)
			{
				bolasjogo--;
				bolas[i].dentro = 0;
				p1->pnt++;
				//bolas[i].vel_y = -bolas[i].vel_y;
			}

			//se a bola bate na parte inferior
			if(bolas[i].y - bolas[i].r >= SCREEN_H)
			{
				bolasjogo--;
				bolas[i].dentro = 0;
				p2->pnt++;
				//bolas[i].vel_y = -bolas[i].vel_y;
			}
			//se a bola esta dentro do Jogador 1
			if(p1->p == 1)
				if(bolas[i].x >= p1->x && bolas[i].x <= p1->x + p1->w)	//se a bola esta nos limites de largura do Jogador 1
					if(bolas[i].y + bolas[i].r >= p1->y && bolas[i].y - bolas[i].r <= p1->y + p1->h) //se a bola esta nos limites de altura do Jogador 1
						if(bolas[i].vel_y > 0){
							p1->p = 0;
							bolas[i].vel_y = -bolas[i].vel_y;
						}
			
			//se a bola esta dentro do Jogador 2
			if(p2->p == 1)
				if(bolas[i].x >= p2->x && bolas[i].x <= p2->x + p2->w)	//se a bola esta nos limites de largura do Jogador 2
					if(bolas[i].y + bolas[i].r >= p2->y && bolas[i].y - bolas[i].r <= p2->y + p2->h && p2->p == 1) //se a bola esta nos limites de altura do Jogador 2
						if(bolas[i].vel_y < 0) {
							p2->p = 0;
							bolas[i].vel_y = -bolas[i].vel_y;
						}
		}
    }
}

int main(int argc, char **argv){

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	/*-----Rotinas de inicialização-----*/
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   
	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());


	//inicia o temporizador
	al_start_timer(timer);


	//Jogadores
	Jogador p1;
	initJogador1(&p1);
    Jogador p2;
	initJogador2(&p2);

	//Bolas
	Bola bolas[20];     //cria uma variável que irá armazenar as bolas
	//geraBolas(bolas);   //gera bolas e armazena nas posições do vetor bolas[]
    
	//Pontuacoes
	FONT_32 = al_load_font("arial.ttf", 32, 1);	//carrega a fonte do arquivo no tamanho 32
	char pntp1[2], pntp2[2];					//cria um char para a pontuacao de cada jogador

	int posicao = 0;	//variavel para armazenar em qual canhao deve ser lancada a bola
	int bolasjogo = 0;	//variavel para armazenar quantas bolas estao no jogo
	int playing = 1;	//variavel para abrir ou fechar o jogo (1 for para abrir / 0 se for pra fechar)
	int tempo = 0;		//variavel para armazenar o tempo em segundos do jogo
	int menu = 0;

	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {

			desenhaQuadra();						    //desenha Quadra
			atualizaJogadores(&p1, &p2);				//atualiza Jogador 1 e Jogador 2
			desenhaJogadores(p1, p2);					//desenha Jogador 1 e Jogador 2
            atualizaBolas(bolas, &p1, &p2, bolasjogo); 	//atualiza as bolas
			desenhaBolas(bolas);						//desenha as bolas
            escrevePontos();                            //Escreve as pontuações na tela

			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();

			//se passou 1 segundo
			if(al_get_timer_count(timer)%(int)FPS == 0) {
				tempo = (al_get_timer_count(timer)/FPS);
				//printf("\n%d segundos se passaram...", tempo);
                printf("%d segundos\n", tempo);
                //a cada TEMP_BOLAS segundos uma bola é lançada de um canhão
	
				if(bolasjogo < 20 && tempo % TEMP_BOLAS == 0)
				{
					canhao(bolas, bolasjogo, posicao);
					printf("lancar no canhao %d\n", posicao);
					if(posicao == 3)
						posicao = 0;
					else
						posicao++;
					bolasjogo++;
					printf("bolas no jogo %d\n\n", bolasjogo);
				}
                
			}
		}

		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		
		//se o tipo de evento for um pressionar de uma tecla - faz com que o jogador comece a se movimentar
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode) {
				/*------JOGADOR 1------*/
				case ALLEGRO_KEY_W:	//caso pressione a tecla W
					p1.cima = 1;
				break;		
				case ALLEGRO_KEY_S:	//caso pressione a tecla S
					p1.baixo = 1;
				break;		
				case ALLEGRO_KEY_A:	//caso pressione a tecla A
					p1.esq = 1;
				break;		
				case ALLEGRO_KEY_D:	//caso pressione a tecla D
					p1.dir = 1;
				break;
				case 75:			//caso pressione a tecla de código 75 (espaco)
					p1.p = 1;
				break;

				/*------JOGADOR 2------*/
                case 84:	//caso pressione a tecla de código 84 (/\)
					p2.cima = 1;
				break;
				case 85:	//caso pressione a tecla de código 85 (\/)
					p2.baixo = 1;
				break;		
				case 82:	//caso pressione a tecla de código 82 (<-)
					p2.esq = 1;
				break;		
				case 83:	//caso pressione a tecla de código 83 (->)
					p2.dir = 1;
				break;
				case 37:	//caso pressione a tecla de código 37 (0)
					p2.p = 1;
				break;
			}
		}

		//se o tipo de evento for soltar uma tecla - faz com que o jogador pare de se movimentar
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {
				/*------JOGADOR 1------*/
				case ALLEGRO_KEY_W:	//caso solte a tecla W
					p1.cima = 0;
				break;		
				case ALLEGRO_KEY_S:	//caso solte a tecla S
					p1.baixo = 0;
				break;
				case ALLEGRO_KEY_A:	//caso solte a tecla A
					p1.esq = 0;
				break;
				case ALLEGRO_KEY_D:	//caso solte a tecla D
					p1.dir = 0;
				break;

				/*------JOGADOR 2------*/
                case 84:	//caso solte a tecla de código 84 (/\)
					p2.cima = 0;
				break;		
				case 85:	//caso solte a tecla de código 85 (\/)
					p2.baixo = 0;
				break;		
				case 82:	//caso solte a tecla de código 82 (<-)
					p2.esq = 0;
				break;		
				case 83:	//caso solte a tecla de código 83 (->)
					p2.dir = 0;
				break;
			}
		}		
	}

	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}