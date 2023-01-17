package cz.cvut.fit.niam1.hateoas.custom;

import cz.cvut.fit.niam1.hateoas.Country;

public class CountryLinks {

    private Country country;
    private CustomLink[] customLinks;

    public CountryLinks(Country country, CustomLink[] customLinks) {
        this.country = country;
        this.customLinks = customLinks;
    }

    public Country getCountry() {
        return country;
    }

    public void setCountry(Country country) {
        this.country = country;
    }

    public CustomLink[] get_links() {
        return customLinks;
    }

    public void set_links(CustomLink[] customLinks) {
        this.customLinks = customLinks;
    }
}
