#include <libxml/xinclude.h>
#include "PiccoloDefinitions.h"
#include <string>

///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// 2621 Wasco St.                //
// PO Box 1500                   //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef PICCOLO_XML_H
#define PICCOLO_XML_H


/*! \file
    \brief XML support routines for writing and reading configuration files

    PiccoloXML hosts the routines to read and write Piccolo configuration data
    to and from XML files.  The XML support is based upon the GNU libxml tools
    version 2.  Any application which uses the XML tools will need to link to
    libxml.

    To use the routines you first open an XML file, then read or write to the
    file.  Finally the file is saved and the XML document tree is freed.

    \code

    BOOL SaveXMLMissionLimits(const MissionLimits_t* pMissionLimits)
    {
        xmlDocPtr doc;

        // Load up the existing document so that we don't lose anything in it
        doc = OpenXMLConfig("MyXMLFile.xml");

        // If the file is bogus then create a new document tree
        if(doc == NULL) doc = CreateNewXMLConfig();

        if(doc == NULL)
        {
            // Failed to create docuemnt tree, configuration not saved"
            return FALSE;
        }

        // Update the mission limits part of the document tree
        SetXMLMissionLimits(doc, pMissionLimits);

        // Save the file
        xmlSaveFile("MyXMLFile.xml", doc);

        // Free the document tree
        xmlFreeDoc(doc);

        return TRUE;
    }// SaveXMLMissionLimits


    BOOL LoadXMLMissionLimits(MissionLimits_t* pMissionLimits)
    {
        std::string ErrorString;
        xmlDocPtr doc;

        // Load up the existing document so that we don't lose anything in it
        doc = OpenXMLConfig("MyXMLFile.xml");

        if(doc == NULL)
        {
            // Failed to open document, data not loaded
            return FALSE;
        }

        // Get the mission limits part of the document
        if(GetXMLMissionLimits(doc, pMissionLimits, ErrorString) == 0)
        {
            // Free the document tree
            xmlFreeDoc(doc);

            return TRUE;
        }
        else
        {
            // Free the document tree
            xmlFreeDoc(doc);

            // Some problem with the file, ErrorString
            //   contains a description of the problem
            return FALSE;
        }

    }// LoadXMLMissionLimits

    \endcode

*/


void AddIndentingElements(xmlNodePtr cur, int IndentLevel);
xmlDocPtr OpenXMLNamedDoc(const char* filename, const char* name);
xmlDocPtr OpenXMLConfig(const char* filename);
xmlDocPtr CreateNewXMLNamedDoc(const char* name);
xmlDocPtr CreateNewXMLConfig(void);
xmlNodePtr FindNamedXMLChildNode(xmlNodePtr parent, const xmlChar* name);
xmlNodePtr FindIndexedNamedXMLChildNode(xmlNodePtr parent, const xmlChar* name, int index);
xmlNodePtr FindNamedXMLSiblingNode(xmlNodePtr Start, const xmlChar* name);
xmlNodePtr FindNextNamedXMLSiblingNode(xmlNodePtr Start, const xmlChar* name);
xmlNodePtr FindOrCreateNamedXMLChildNode(xmlNodePtr parent, const xmlChar* name, const xmlChar* content = NULL);
xmlNodePtr FindOrCreateIndexedNamedXMLChildNode(xmlNodePtr parent, const xmlChar* name, int index, const xmlChar* content = NULL);
BOOL ReadXMLDataContent(xmlDocPtr doc, xmlNodePtr root, const char* name, float* pValue);
BOOL ReadXMLDataContent(xmlDocPtr doc, xmlNodePtr root, const char* name, double* pValue);
BOOL ReadXMLDataContent(xmlDocPtr doc, xmlNodePtr root, const char* name, UInt8* pValue);
BOOL ReadXMLDataContent(xmlDocPtr doc, xmlNodePtr root, const char* name, UInt16* pValue);
BOOL ReadXMLDataContent(xmlDocPtr doc, xmlNodePtr root, const char* name, UInt32* pValue);
BOOL ReadXMLDataContent(xmlDocPtr doc, xmlNodePtr root, const char* name, SInt8* pValue);
BOOL ReadXMLDataContent(xmlDocPtr doc, xmlNodePtr root, const char* name, SInt16* pValue);
BOOL ReadXMLDataContent(xmlDocPtr doc, xmlNodePtr root, const char* name, SInt32* pValue);
BOOL WriteXMLDataContent(xmlNodePtr root, const char* name, char* content);
BOOL WriteXMLDataContent(xmlNodePtr root, const char* name, double Value);
BOOL WriteXMLDataContent(xmlNodePtr root, const char* name, double Value, int precision);
BOOL WriteXMLDataContent(xmlNodePtr root, const char* name, float Value);
BOOL WriteXMLDataContent(xmlNodePtr root, const char* name, UInt8 Value);
BOOL WriteXMLDataContent(xmlNodePtr root, const char* name, UInt16 Value);
BOOL WriteXMLDataContent(xmlNodePtr root, const char* name, UInt32 Value);

BOOL SetXMLMissionLimits(xmlDocPtr doc, const MissionLimits_t* pMissionLimits);
UInt32 GetXMLMissionLimits(xmlDocPtr doc, MissionLimits_t* pMissionLimits, std::string& Error, const MissionLimits_t* pToCompare = NULL);
BOOL SetXMLEuler(xmlDocPtr doc, const float Euler[NAXIS], const float IMUtoAnt[N3D], const float IMUtoAnt2[N3D]);
UInt32 GetXMLEuler(xmlDocPtr doc, float Euler[NAXIS], float IMUtoAnt[N3D], float IMUtoAnt2[N3D], std::string& Error, 
                   const float* pEulerToCompare = NULL, const float* pIMUtoAntToCompare = NULL, const float* pIMUtoAnt2ToCompare = NULL);
BOOL SetXMLSurfaces(xmlDocPtr doc, const ActuatorTable_t Table[NUM_SURFACES], const UInt8 Types[NUM_SURFACES],
                    const UInt8 Virtual[NUM_SURFACES], const UInt8 Fast[NUM_SURFACES]);
UInt32 GetXMLSurfaces(xmlDocPtr doc, ActuatorTable_t Table[NUM_SURFACES], UInt8 Types[NUM_SURFACES], 
                      UInt8 Virtual[NUM_SURFACES], UInt8 Fast[NUM_SURFACES], 
                      std::string& Error, 
                      const ActuatorTable_t* pTableToCompare = NULL, const UInt8* pTypesToCompare = NULL,
                      const UInt8* pVirtualToCompare = NULL, const UInt8* pFastToCompare = NULL);
BOOL SetXMLPayload1(xmlDocPtr doc, const ExternalIO_t ExternalIO[NUM_PAYLOAD_IO_PINS], UInt32 SampleTime);
UInt32 GetXMLPayload1(xmlDocPtr doc, ExternalIO_t ExternalIO[NUM_PAYLOAD_IO_PINS], UInt32* pSampleTime, std::string& Error);
BOOL SetXMLPayload2(xmlDocPtr doc, const ExternalIO_t ExternalIO[NUM_PAYLOAD2_IO_PINS], UInt32 SampleTime);
UInt32 GetXMLPayload2(xmlDocPtr doc, ExternalIO_t ExternalIO[NUM_PAYLOAD2_IO_PINS], UInt32* pSampleTime, std::string& Error);
BOOL SetXMLPayload(xmlDocPtr doc, const ExternalIO_t ExternalIO[NUM_EXTERNAL_IO_PINS], UInt32 SampleTime);
UInt32 GetXMLPayload(xmlDocPtr doc, ExternalIO_t ExternalIO[NUM_EXTERNAL_IO_PINS], UInt32* pSampleTime, std::string& Error, const ExternalIO_t* pExternalIOToCompare = NULL, const UInt32* pSampleTimeToCompare = NULL);
BOOL SetXMLExternalSerial(xmlDocPtr doc, const ExternalSerial_t* pSerial);
UInt32 GetXMLExternalSerial(xmlDocPtr doc, ExternalSerial_t* pSerial, std::string& Error, const ExternalSerial_t* pToCompare = NULL);
BOOL SetXMLMagCal(xmlDocPtr doc, const float HardIronErr[N3D], const float ScaleFactErr[N3D]);
UInt32 GetXMLMagCal(xmlDocPtr doc, float HardIronErr[N3D], float ScaleFactErr[N3D], std::string& Error, const float* pHardIronToCompare = NULL, const float* pScaleFactToCompare = NULL);
BOOL SetXMLSensorError(xmlDocPtr doc, const SensorErrorData_t* pSensorError);
UInt32 GetXMLSensorError(xmlDocPtr doc, SensorErrorData_t* pSensorError, std::string& Error, const SensorErrorData_t* pSensorErrorCmp = NULL);

#endif // PICCOLO_XML_H
