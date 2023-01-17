package cz.cvut.fit.niam1.hateoas.custom;

public class CustomLink {

    private String href;
    private String rel;

    public CustomLink(String href, String rel) {
        this.href = href;
        this.rel = rel;
    }

    public String getHref() {
        return href;
    }

    public void setHref(String href) {
        this.href = href;
    }

    public String getRel() {
        return rel;
    }

    public void setRel(String rel) {
        this.rel = rel;
    }
}
