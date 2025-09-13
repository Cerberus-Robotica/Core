//
// Created by caiu on 13/09/25.
//

#ifndef CIRCULARSHADOW_H
#define CIRCULARSHADOW_H

#include "Shadow.h"
#include "AreaCircular.h"

class CircularShadow : public Shadow {
private:
	AreaCircular circular;
public:
	CircularShadow();
};



#endif //CIRCULARSHADOW_H
