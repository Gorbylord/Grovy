#include <stdio.h>
#include <time.h>
#include <Windows.h>


int update()
{
	printf ("Updeteando\n");
	if (GetAsyncKeyState(VK_ESCAPE))
		return 1;
	return 0;
}
int main()
{
	bool condition = true;
	printf ("hell yeah!");
	while (condition) {
		condition = (update() == 0);
	}
	return 0;
}