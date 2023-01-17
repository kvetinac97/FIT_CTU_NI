package cz.cvut.fit.niam1;

import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@Component
public class BalancerRepository {

    private final List<ServerInfo> servers;

    public BalancerRepository() {
        this.servers = new ArrayList<>();
        servers.add(new ServerInfo("http://147.32.233.18:8888/MI-MDW-LastMinute1/list", false));
        servers.add(new ServerInfo("http://147.32.233.18:8888/MI-MDW-LastMinute2/list", false));
        servers.add(new ServerInfo("http://147.32.233.18:8888/MI-MDW-LastMinute3/list", false));
    }

    public List<ServerInfo> getRunningServers() {
        return servers.stream().filter(ServerInfo::isActive).collect(Collectors.toList());
    }

    public List<ServerInfo> getServers() {
        return servers;
    }

}
