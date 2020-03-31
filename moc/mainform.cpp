#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qlistview.h>
#include <qfile.h>
#include "types.h"
#include "mainform.h"
#include "product.h"
#include "regime.h"
#include "iptraffic.h"
#include "struct_all.h"

#define rus(x) QString::fromLocal8Bit(x)
#define RUS(x) ((x).local8Bit())

//==========================================================================
MainForm::MainForm( QWidget* parent, const char* name, WFlags fl)
{
	parent = parent;
	name = name;
	fl = fl;
	return;
}

void MainForm::connectToProduct()
{
	ProductForm Productivity;
	Productivity.exec();
	return;
}

void MainForm::connectToRegime()
{
	RegimeAddressForm Regime;
	Regime.exec();
	return;
}

void MainForm::connectToIPData()
{
	IPTrafficForm IP_traffic;
	IP_traffic.exec();
	return;
}
