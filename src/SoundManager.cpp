#include "SoundManager.h"
#include "maths.h"


FMOD::System *Soundsystem = NULL;
typedef FMOD::Sound* SoundItem;
vector <SoundItem*> sounds;
FMOD_RESULT result;

bool SoundManager::init(){
	result = FMOD::System_Create(&Soundsystem);      // Create the main system object.
	if (result != FMOD_OK)
	{
		printf("FMOD error creating system! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	int driverCount = 0;
	result = Soundsystem->getNumDrivers(&driverCount);

	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}


	result = Soundsystem->init(32, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		printf("FMOD error initialising! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	return true;
}

void SoundManager::Update(){
result = Soundsystem->update();

if (result != FMOD_OK) {
	printf("FMOD error updating! (%d) %s\n", result, FMOD_ErrorString(result));
	exit(-1);
	}

}

void SoundManager::createSound(const char* filename, SoundItem*  pSound)
{

result = Soundsystem->createSound(filename, FMOD_DEFAULT, 0, pSound);

if (result != FMOD_OK){
	printf("FMOD error creating sound! (%d) %s\n", result, FMOD_ErrorString(result));
	exit(-1);
}
}


void SoundManager::ReleaseSound(SoundItem pSound)
{
	result = pSound->release();
	if (result != FMOD_OK){
		printf("FMOD error releasing sound! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

}


void SoundManager::playsound(SoundItem psound, bool bLoop = false, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 vel = glm::vec3(0, 0, 0)){
	

	if (!bLoop)
		psound->setMode(FMOD_LOOP_OFF);
	else
	{
		psound->setMode(FMOD_LOOP_NORMAL);
		psound->setLoopCount(-1);
	}

	Soundsystem->playSound(psound,NULL, false, 0);
}

void SoundManager::stopsound(){
	FMOD_Channel_Stop(NULL);
}

void SoundManager::shutdown(){
	Soundsystem->close();
	Soundsystem->release();
}
