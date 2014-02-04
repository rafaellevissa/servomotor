//Para compilar no Visual Studio: cl -EHsc serial.cpp

#include	<iostream>
#include	<cstdlib>
#include	<math.h>
#include	<Windows.h>
#include	<conio.h>

#define LEN_BUFFER  100  //Tamanho do Buffer.

using namespace std;
bool abrirPorta(void);
bool configuraPorta(void);
bool configuraTimeouts(void);
void escreveByte (char *dado);

HANDLE  hCom; // Handle para a Porta Serial (identificador).
DCB dcb; //Estrutura DCB é utilizada para definir todos os parâmetros da comunicação.
COMMTIMEOUTS CommTimeouts; 

int main(void)
{
	char caracter[2] = "o";
	system("CLS");
	if (abrirPorta())
	{	
		cout << "Porta COM3 aberta com sucesso\n";
		if(configuraPorta())
			cout << "Porta configurada\n";
		else
			cout << "Erro ao tentar configurar a porta\n";
		if (configuraTimeouts())
			cout << "Timeouts configurado\n";
		else
			cout << "Erro ao tentar configurar timeouts\n";
		
		system("PAUSE");
		
		while(caracter[0] != 'q')
		{
			system("CLS");
			cout << "****************************\n";
			cout << "   Controle do ServoMotor   \n";
			cout << "****************************\n";
			cout << " <- : Direita \n";
			cout << " -> : Esquerda \n";
			cout << " q  : Encerrar comunicacao\n";
			cout << "****************************\n";
			caracter[0] = getch();
			escreveByte(caracter);
		}
		if (CloseHandle( hCom ))  //Fecha a porta aberta anteriormente por CreateFile().
			cout << "Porta COM3 fechada com sucesso\n";
		else 
			cout << "A porta COM3 não foi fechada\n";
	}
	else
		cout << "Erro ao tentar abrir a porta COM3\n"; 		
	system("PAUSE");
	return EXIT_SUCCESS;
}
bool abrirPorta(void)
{
	char *NomePorta = "COM4";  //COM1, COM2...COM9 ou portas virtuais "\\.\COMx".
	hCom = CreateFile(
						NomePorta,   //Nome da porta.
                        GENERIC_READ | GENERIC_WRITE,   //Para leitura e escrita.
                        0,       //(Zero) Nenhuma outra abertura será permitida.
                        NULL,  //Atributos de segurança. (NULL) padrão.
                        OPEN_EXISTING,  //Criação ou abertura. 
                        0,         //Entrada e saída sem overlapped.
                        NULL     //Atributos e Flags. Deve ser NULL para COM.
                     );
	if(hCom == INVALID_HANDLE_VALUE)
		return false;  //Erro ao tentar abrir a porta especificada.  
	else 
		return true;
}
bool configuraPorta(void)
{
	if( !GetCommState(hCom, &dcb))	
		return false;  // Erro na leitura de DCB.
	dcb.BaudRate = CBR_9600; 
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT; 
	//Define novo estado. 
	if( SetCommState(hCom, &dcb) == 0 )
		return false;   //Erro.
	return true;
} 
bool configuraTimeouts(void)
{
	if( GetCommTimeouts(hCom, &CommTimeouts) == 0 )
		return false;   //Erro.

    //Define novos valores.
	CommTimeouts.ReadIntervalTimeout = 2;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 2;
	CommTimeouts.WriteTotalTimeoutMultiplier = 5;
	CommTimeouts.WriteTotalTimeoutConstant = 5;

	if( SetCommTimeouts(hCom, &CommTimeouts) == 0 )
		return false;   //Erro.
	return true;
}
void escreveByte (char *dado)
{
    DWORD BytesEscritos = 0;
	int TamaString;    //Para armazenar o tamanho da String.
	char BufferEnvia[LEN_BUFFER];    //Para armazenar a string a ser enviada.
	strcpy(BufferEnvia, dado);  //Prepara a string a ser enviada.
	TamaString = strlen(BufferEnvia); //Calcula o tamanho da string a ser enviada.

	if(!WriteFile( hCom, BufferEnvia, TamaString, &BytesEscritos, NULL ))
	{
		cout << "Ocorreu um erro ao tentar escrever o dado\n";	//error occurred. Report to user.
	}
	return;
}
