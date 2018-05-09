#pragma once

#include <map>
#include "lib.h"

#define SWID_NS "http://standards.iso.org/iso/19770/-2/2015/schema.xsd"


enum {SWID_ELEMENT_ENTITY = 0, SWID_ELEMENT_LINK, TOTAL_SWID_ELEMENT_COUNT};


template<class el_t>
class XMLIO : public SWIDTagIO
{
public:
	virtual ~XMLIO() override;

	virtual SWIDStruct load(const std::string & filename) override;
	virtual void save(const std::string & filename, const SWIDStruct & what) override;

protected:
	virtual void setAttrValue(el_t * el, const char * name, const std::string & value) = 0;
	virtual std::string extractAttrValue(el_t * el, const char * name) const = 0;
	virtual std::map<int, std::vector<el_t *> > subElementsOf(el_t * el) const = 0;
	virtual el_t * createRoot() = 0;
	virtual el_t * readRoot(const std::string & filename) = 0;
	virtual el_t * createSubElement(el_t * parent, int element_type) = 0;
	virtual void saveToFile(const std::string & filename) = 0;

	std::map<int, std::vector<el_t *> > createEmptyMap() const;
private:
	void addEntities(SWIDStruct & subject, const std::vector<el_t *> & entities);
	void addLinks(SWIDStruct & subject, const std::vector<el_t *> & links);
};
