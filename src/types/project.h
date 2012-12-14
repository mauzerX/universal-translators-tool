/***************************************************************
 * Name:      project.h
 * Purpose:   declaration of Project container class
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-12-12
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
#ifndef PROJECT_H_INCLUDED
#define PROJECT_H_INCLUDED



#include "istatestore.h"
#include "types/ieditor.h"

enum IECommands
{
	iecImport,
	iecExport,
	iecNum
};

extern const wxString sCommandNames[iecNum];



class Project: public IStateStore
{
public:

	Project();
	Project::Project( const Project& other );
	~Project();

	bool IsChanged() const { return mChanged; }
	int CheckChanged();

	bool CreateProject( const wxString& fullPath );
	bool SaveProject();

	bool IsAllowed( IECommands what, EditorType who );
	bool Do( IECommands what, EditorType who );

protected:

	virtual bool SaveState( wxOutputStream& output );
	virtual bool LoadState( wxInputStream& input, int version );
	
private:

	wxString		GetFunctionName( IECommands what, EditorType who );

	bool			mChanged;
	wxString		mProjectName;
	wxString		mModuleName;
	wxString		mModuleVersion;
	wxString		mLastDir;
	wxString		mProjectFileName;


	EditorsArray	mEditors;
};

#endif // PROJECT_H_INCLUDED
