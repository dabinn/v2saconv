#include "Search.h"
#include "ModelConverter.h"
#include "TextureConverter.h"
#include <new>
#include "Log.h"

void ConvertYTD(char *filepath)
{
	LOGL("Converting ytd: %s", filepath);
	char txdName[MAX_PATH];
	_splitpath(filepath, NULL, NULL, txdName, NULL);
	char txdpath[MAX_PATH];
	strcpy(txdpath, filepath);
	if(settings.m_bHashedFileNames)
		sprintf(&strrchr(txdpath, '\\')[1], "%X.txd", HASH(txdName));
	else
	{
		strcpy(&txdpath[strlen(txdpath) - 3], "txd");
		if(strlen(txdName) > 20)
			LOGL("  WARNING: File \"%s\" has too long name. Rename this file manually or use \"bHashedFileNames\" option", txdName);
	};
	texture_converter::convert_ytd_to_txd(filepath, txdpath);
}

void ConvertYDR(char *filepath)
{
	LOGL("Converting ydr: %s", filepath);
	char modelName[MAX_PATH];
	_splitpath(filepath, NULL, NULL, modelName, NULL);
	char dffpath[MAX_PATH];
	strcpy(dffpath, filepath);
	//model_converter::convert_ydr_to_dff(filepath, dffpath, modelName);
	model_converter::convert_ydr_to_obj(filepath, dffpath, modelName);
}

void ConvertYDD(char *filepath)
{
	LOGL("Converting ydd: %s", filepath);
	model_converter::convert_ydd_to_dff(filepath);
}

void ConvertYFT(char *filepath)
{
	LOGL("Converting yft: %s", filepath);
	char modelName[MAX_PATH];
	_splitpath(filepath, NULL, NULL, modelName, NULL);
	char dffpath[MAX_PATH];
	strcpy(dffpath, filepath);
	if(settings.m_bHashedFileNames)
	{
		sprintf(modelName, "%X", HASH(modelName));
		sprintf(&strrchr(dffpath, '\\')[1], "%s.dff", modelName);
	}
	else
	{
		strcpy(&dffpath[strlen(dffpath) - 3], "dff");
		if(!settings.m_bHashedFileNames && strlen(modelName) > 20)
			LOGL("  WARNING: File \"%s\" has too long name. Rename this file manually or use \"bHashedFileNames\" option", modelName);
		modelName[23] = '\0';
	}
	model_converter::convert_yft_to_dff(filepath, dffpath, modelName);
}

int main(int argc, char *argv[])
{
	SetConsoleTitle("YTDYDRYDDYFT2TXDDFFCOL");
	if(settings.m_bEnableLog)
		LOG_OPEN();
	
	if (argc > 1)
	{
		printf("argc=%d\n", argc);
		UINT8 i;
		for ( i= 1; i < argc; i++)
		{
			if (argv[i]!=NULL)
				printf("arg%d\t%s\n", i, argv[i]);
		}

		char ext[4];
		i = strlen(argv[1]);
		ext[0] = argv[1][i - 3];
		ext[1] = argv[1][i - 2];
		ext[2] = argv[1][i - 1];
		ext[3] = 0;
		//strcpy(ext, argv[1]+strlen(argv[1]) - 3);
		if (strcmp(ext,"ydr")==0)
		{
			ConvertYDR(argv[1]);
		}

	}
	else{
		LOGL("No arguments. Searching file..");
		LOGL("Searching for ytd...");
		SearchFiles("*.ytd", (LPSEARCHFUNC)ConvertYTD);
		LOGL("Searching for ydr...");
		SearchFiles("*.ydr", (LPSEARCHFUNC)ConvertYDR);
		LOGL("Searching for ydd...");
		SearchFiles("*.ydd", (LPSEARCHFUNC)ConvertYDD);
		LOGL("Searching for yft...");
		SearchFiles("*.yft", (LPSEARCHFUNC)ConvertYFT);

	}

	LOG_CLOSE();
	printf("\nConversion finished.");

	getchar();
	return 1;
}