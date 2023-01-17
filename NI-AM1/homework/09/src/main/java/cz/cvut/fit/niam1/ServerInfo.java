package cz.cvut.fit.niam1;

public class ServerInfo {

    public final String url;
    public Boolean active;

    public ServerInfo(String url, Boolean active) {
        this.url = url;
        this.active = active;
    }

    public String getUrl() {
        return url;
    }

    public Boolean isActive() {
        return active;
    }

    public void setActive(Boolean active) {
        this.active = active;
    }
}
