#include "externs.h"

#define City(X)                  location.push_back(city = new Location(X)); \
    city->id = id++;

#define District(X, Y)           district = city->addchild(X); \
    district->id = id++; \
    district->area = Y;

#define DistrictProperty(X, Y)   district->X = Y;
#define DistrictName(Y)          strcpy(district->name, Y);
#define DistrictShortname(Y)     strcpy(district->shortname, Y);

#define Site(X)                  site = district->addchild(X); \
    site->id = id++;

#define SiteProperty(X, Y)       site->X = Y;
#define SiteName(Y)              strcpy(site->name, Y);
#define SiteShortname(Y)         strcpy(site->shortname, Y);

Location *find_site_by_id(int id) {
    for(int i = 0; i < location.size(); i++)
        if(location[i]->id == id)
            return location[i];

    return 0;
}

Location *find_site_in_city(int site, int city) {
    for(int i = 0; i < location.size(); i++)
        if(location[i]->type == site && location[i]->city == city)
            return location[i];

    return 0;
}

void make_world() {
    //MAKE LOCATIONS
    Location *city;
    Location *district;
    Location *site;
    int id = 0;

    // Seattle
    City(SITE_CITY_SEATTLE)
    District(SITE_DOWNTOWN, 0)
    //DistrictProperty(mapped, hasmaps)
    Site(SITE_RESIDENTIAL_APARTMENT_UPSCALE)
    Site(SITE_GOVERNMENT_POLICESTATION)
    Site(SITE_GOVERNMENT_COURTHOUSE)
    Site(SITE_BUSINESS_BANK)
    Site(SITE_GOVERNMENT_FIRESTATION)
    Site(SITE_MEDIA_AMRADIO)
    Site(SITE_BUSINESS_CIGARBAR)
    Site(SITE_BUSINESS_LATTESTAND)
    Site(SITE_BUSINESS_DEPTSTORE)
    Site(SITE_BUSINESS_BARANDGRILL)
    SiteProperty(renting, RENTING_CCS)
    SiteProperty(hidden, true)
    SiteProperty(mapped, false)
    District(SITE_UDISTRICT, 0)
    Site(SITE_RESIDENTIAL_APARTMENT)
    Site(SITE_HOSPITAL_UNIVERSITY)
    Site(SITE_HOSPITAL_CLINIC)
    Site(SITE_LABORATORY_GENETIC)
    Site(SITE_LABORATORY_COSMETICS)
    Site(SITE_BUSINESS_VEGANCOOP)
    Site(SITE_BUSINESS_JUICEBAR)
    Site(SITE_BUSINESS_INTERNETCAFE)
    Site(SITE_OUTDOOR_PUBLICPARK)
    Site(SITE_BUSINESS_HALLOWEEN)
    District(SITE_INDUSTRIAL, 0)
    Site(SITE_RESIDENTIAL_SHELTER)
    SiteProperty(renting, RENTING_PERMANENT)
    Site(SITE_INDUSTRY_WAREHOUSE)
    SiteProperty(renting, RENTING_PERMANENT)
    SiteProperty(upgradable, true)
    Site(SITE_RESIDENTIAL_TENEMENT)
    Site(SITE_INDUSTRY_POLLUTER)
    Site(SITE_INDUSTRY_SWEATSHOP)
    Site(SITE_BUSINESS_CRACKHOUSE)
    SiteProperty(upgradable, true)
    Site(SITE_BUSINESS_PAWNSHOP)
    Site(SITE_BUSINESS_CARDEALERSHIP)
    District(SITE_OUTOFTOWN, 1)
    Site(SITE_GOVERNMENT_PRISON)
    Site(SITE_GOVERNMENT_INTELLIGENCEHQ)
    //Site(SITE_INDUSTRY_NUCLEAR)
    Site(SITE_CORPORATE_HEADQUARTERS)
    //Site(SITE_CORPORATE_HOUSE)
    Site(SITE_GOVERNMENT_ARMYBASE)
    // New York City
    City(SITE_CITY_NEW_YORK)
    District(SITE_DOWNTOWN, 0)
    DistrictName("Manhattan Island")
    DistrictShortname("Manhattan")
    //DistrictProperty(mapped, hasmaps)
    Site(SITE_RESIDENTIAL_APARTMENT_UPSCALE)
    Site(SITE_GOVERNMENT_POLICESTATION)
    Site(SITE_GOVERNMENT_COURTHOUSE)
    Site(SITE_BUSINESS_BANK)
    Site(SITE_CORPORATE_HEADQUARTERS)
    Site(SITE_MEDIA_AMRADIO)
    Site(SITE_MEDIA_CABLENEWS)
    Site(SITE_BUSINESS_CIGARBAR)
    //Site(SITE_CORPORATE_HOUSE)
    //Site(SITE_GOVERNMENT_INTELLIGENCEHQ)
    Site(SITE_OUTDOOR_PUBLICPARK)
    Site(SITE_BUSINESS_DEPTSTORE)
    Site(SITE_GOVERNMENT_PRISON)
    District(SITE_UDISTRICT, 0)
    DistrictName("Brooklyn and Queens")
    DistrictShortname("Long Island")
    Site(SITE_INDUSTRY_WAREHOUSE)
    SiteProperty(renting, RENTING_PERMANENT)
    SiteProperty(upgradable, true)
    Site(SITE_RESIDENTIAL_APARTMENT)
    Site(SITE_GOVERNMENT_FIRESTATION)
    Site(SITE_HOSPITAL_UNIVERSITY)
    Site(SITE_HOSPITAL_CLINIC)
    Site(SITE_BUSINESS_JUICEBAR)
    Site(SITE_BUSINESS_INTERNETCAFE)
    Site(SITE_INDUSTRY_POLLUTER)
    Site(SITE_LABORATORY_GENETIC)
    Site(SITE_GOVERNMENT_ARMYBASE)
    Site(SITE_RESIDENTIAL_BOMBSHELTER)
    SiteProperty(renting, RENTING_CCS)
    SiteProperty(hidden, true)
    District(SITE_INDUSTRIAL, 0)
    DistrictName("The Bronx")
    DistrictShortname("The Bronx")
    Site(SITE_RESIDENTIAL_SHELTER)
    SiteProperty(renting, RENTING_PERMANENT)
    Site(SITE_RESIDENTIAL_TENEMENT)
    Site(SITE_INDUSTRY_POLLUTER)
    Site(SITE_INDUSTRY_SWEATSHOP)
    Site(SITE_LABORATORY_COSMETICS)
    Site(SITE_BUSINESS_VEGANCOOP)
    Site(SITE_BUSINESS_PAWNSHOP)
    Site(SITE_BUSINESS_CARDEALERSHIP)
    Site(SITE_BUSINESS_CRACKHOUSE)
    SiteProperty(upgradable, true)
    Site(SITE_OUTDOOR_PUBLICPARK)
    District(SITE_OUTOFTOWN, 1)
    Site(SITE_INDUSTRY_NUCLEAR)
    // Los Angeles
    City(SITE_CITY_LOS_ANGELES)
    District(SITE_DOWNTOWN, 0)
    //DistrictProperty(mapped, hasmaps)
    Site(SITE_RESIDENTIAL_SHELTER)
    SiteProperty(renting, RENTING_PERMANENT)
    Site(SITE_RESIDENTIAL_APARTMENT)
    Site(SITE_GOVERNMENT_POLICESTATION)
    Site(SITE_GOVERNMENT_COURTHOUSE)
    Site(SITE_BUSINESS_BANK)
    Site(SITE_GOVERNMENT_FIRESTATION)
    Site(SITE_CORPORATE_HEADQUARTERS)
    Site(SITE_HOSPITAL_UNIVERSITY)
    Site(SITE_BUSINESS_DEPTSTORE)
    District(SITE_UDISTRICT, 0)
    DistrictName("Greater Hollywood")
    DistrictShortname("Hollywood")
    Site(SITE_RESIDENTIAL_APARTMENT_UPSCALE)
    Site(SITE_BUSINESS_VEGANCOOP)
    Site(SITE_BUSINESS_HALLOWEEN)
    Site(SITE_BUSINESS_CIGARBAR)
    //Site(SITE_MEDIA_CABLENEWS)
    Site(SITE_MEDIA_AMRADIO)
    Site(SITE_OUTDOOR_PUBLICPARK)
    Site(SITE_CORPORATE_HOUSE)
    District(SITE_INDUSTRIAL, 0)
    DistrictName("Seaport Area")
    DistrictShortname("Seaport")
    Site(SITE_INDUSTRY_WAREHOUSE)
    SiteProperty(renting, RENTING_PERMANENT)
    SiteProperty(upgradable, true)
    Site(SITE_RESIDENTIAL_TENEMENT)
    Site(SITE_HOSPITAL_CLINIC)
    Site(SITE_LABORATORY_GENETIC)
    Site(SITE_LABORATORY_COSMETICS)
    Site(SITE_INDUSTRY_POLLUTER)
    Site(SITE_BUSINESS_PAWNSHOP)
    Site(SITE_INDUSTRY_SWEATSHOP)
    Site(SITE_BUSINESS_CARDEALERSHIP)
    Site(SITE_BUSINESS_CRACKHOUSE)
    SiteProperty(upgradable, true)
    District(SITE_OUTOFTOWN, 1)
    DistrictName("Outskirts and Orange County")
    Site(SITE_GOVERNMENT_PRISON)
    //Site(SITE_GOVERNMENT_INTELLIGENCEHQ)
    Site(SITE_INDUSTRY_NUCLEAR)
    Site(SITE_GOVERNMENT_ARMYBASE)
    Site(SITE_OUTDOOR_BUNKER)
    SiteProperty(renting, RENTING_CCS)
    SiteProperty(hidden, true)


    //City(SITE_CITY_CHICAGO);
    //City(SITE_CITY_DETROIT);
    //City(SITE_CITY_ATLANTA);
    //City(SITE_CITY_MIAMI);
    //City(SITE_CITY_WASHINGTON_DC);
}