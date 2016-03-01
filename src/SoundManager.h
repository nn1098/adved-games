#include <fmod\fmod.h>
#include <fmod\fmod.hpp>
#include <fmod\fmod_common.h>
#include <fmod\fmod_errors.h>
#include "maths.h"
#include "filesystem.h"
#include <vector>


using namespace std;
class SoundManager{
public:
	SoundManager();
	~SoundManager();
	bool init();
	typedef FMOD::Sound* SoundItem;
	void ReleaseSound(SoundItem psound);
	void createSound(const char* filename, SoundItem*  pSound);
	void playsound(SoundItem psound, bool bLoop, glm::vec3 pos,  glm::vec3 vel);
	void stopsound();
	void Update();
	void shutdown();

};

