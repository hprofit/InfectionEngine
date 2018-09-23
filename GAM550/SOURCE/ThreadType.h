/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef THREADTYPE_H
#define THREADTYPE_H

// ThreadType specifies what the general purpose of a thread is
// Ex. The thread dedicated to running rendering tasks would be of RenderingThread type
// This is utilized in the ThreadCommand system via the JobManager to give these
// commands to specific threads rather than spawning smaller threads to do some work

enum ThreadType {
	TT_Thread0 = 0,
	TT_Thread1,
	TT_Thread2,
	TT_Thread3,
	AnyThread,
	NUMTHREADTYPES
};

#endif