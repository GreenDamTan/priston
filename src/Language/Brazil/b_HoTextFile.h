#include "..\\HoBaram\\HoTextFile.h"

char *HoTextLoginMessage[18] =  {"Estabelecendo a conex�o....",
				"N�o foi poss�vel se conectar ao servidor",
				"     Digite sua ID novamente    ",
				" Digite sua senha novamente  ",
				"ID bloqueada, visite nosso site",
				"  Essa ID j� est� logada  ",
				"  Vers�o n�o compat�vel   ",
				"      Servidor cheio       ",
				"Conex�o perdida com o servidor",
				"   Tempo limite excedido  ",
				"   Tempo limite excedido   ",
				" Deletando a conta no servidor ",
				 "Servidor sobrecarregado!!!, por favor, tente mais tarde.",
				" Por favor, tente mais tarde... ",
				"Senha incorreta por mais de 3 vezes.",
				"Voc� n�o pode usar esses caracteres na senha.",
				"allow",
				};

char *HoTextSelectMessage[12] = {"Voc� deseja deletar?",
				  "Continuar esta aventura?",
				  "Limite de personagens.",
				  "Cria��o completa. Confirmar?", //3
				  "Digite um nome", //4
				  "Esse nome j� existe", //5
				  "  Selecione um personagem   ", //6
				  "    Selecione uma tribo     ", //7
				  "    Selecione uma classe     ", //8
				  "     Selecione a face     ", //9
				  "Imposs�vel deletar L�der do Clan", //10
				  "Dados do Clan n�o recebidos" //11
				};

char *HoTextTempscronInfo[5] = {" Os Tempskrons s�o a civiliza��o mais avan�ada do continente de Priston, sendo capazes de controlar todos os tipos de equipamentos, at� mesmo os mais modernos.",
								/* Maybe " A tribo dos Tempskrons ,famosa por suas t�nicas de combate e sua grande habilidade com v�ios tipos de armas"*/
                                " Conhecida pela sua tecnologia avan�ada, a tribo dos Tempskrons � a un�ca capaz de usar m�quinas modernas a favor do combate.",
								" Os Lutadores s�o guerreiros natos, sua especialidade � a luta com Machados...",
								" Personagens eficientes, podem usar suas t�cnicas a favor de uma luta de qualidade,.",
								" Mestres na arte da luta a dist�ncia, s�o as pioneiras com o Arco e Flecha."};



char *HoTextMoryon = "Morion";
char *HoTextMoryonInfo[5] = {
	"Os Morions residem na parte Norte do continente de Priston. S�o ing�nuos e d�ceis. Sua cultura prospera. Valorizam ao extremo ambos os treinamentos - o espiritual e o f�sico. S�o os pioneiros na arte de usar as for�as da magia e da ora��o, a favor do combate.",
        "Com suas armaduras reluzentes e suas espadas poderosas, representam a classe Top dos Guerreiros dos Morions. Eles usam a for�a sagrada para derrotar os inimigos do mal.",
	"Atalantas s�o profissionais na arte das Lan�as. Sua t�cnica � perfeita quando o assunto � arremesso. Com seus poderes magn�ficos, o estrago dos ataques � ainda maior.",
	"As Sacerdotisas s�o as rainhas da magia, usando sua forca para combater o mal, seja atacando, seja curando. Mesmo com seu alto poder de cura, s�o consideravelmente vulner�veis ",
	"Magos convertem toda a for�a da natureza em ataques brutais. Com conhecimento anormal acerca dos elementos, s�o os melhores quando o assunto � Magia. Tamb�m s�o famosos por controlar a terra e as estrelas.",
};


char *HoText_PartyMessage = "enviou um pedido de grupo";

char *HoTextTempskron = "Tempskron";

char *HoTextFriendNear = "[Recentes]";
char *HoTextFriendList = "[Amigos] %d/%d";
char *HoTextFriendRefuse = "[Ignore] %d/%d";