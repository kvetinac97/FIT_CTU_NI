k8s-demo-ver:
	KUBECONFIG=./kube_config_cluster.yml ./kubectl get nodes

k8s-demo-setup-cert-manager:
	#CRD
	KUBECONFIG=./kube_config_cluster.yml ./kubectl apply --validate=false -f https://github.com/jetstack/cert-manager/releases/download/v1.11.1/cert-manager.crds.yaml

k8s-demo-install-cert-manager:
	KUBECONFIG=./kube_config_cluster.yml ./kubectl create namespace cert-manager
	KUBECONFIG=./kube_config_cluster.yml ./helm repo add jetstack https://charts.jetstack.io
	KUBECONFIG=./kube_config_cluster.yml ./helm repo update
	KUBECONFIG=./kube_config_cluster.yml ./helm install \
  	cert-manager jetstack/cert-manager \
  	--namespace cert-manager \
  	--version v1.11.1

k8s-demo-setup-rancher:
	KUBECONFIG=./kube_config_cluster.yml ./helm repo add rancher-latest https://releases.rancher.com/server-charts/latest

k8s-demo-install-rancher:
	KUBECONFIG=./kube_config_cluster.yml ./kubectl create namespace cattle-system --dry-run=client -o yaml | ./kubectl apply -f -
	KUBECONFIG=./kube_config_cluster.yml ./helm upgrade --install rancher rancher-latest/rancher --set global.cattle.psp.enabled=false \
	--namespace cattle-system \
	--set hostname=klastr.demo

k8s-demo-verify-install-rancher:
	KUBECONFIG=./kube_config_cluster.yml ./kubectl -n cattle-system rollout status deploy/rancher
	echo ""
	KUBECONFIG=./kube_config_cluster.yml ./kubectl -n cattle-system get deploy rancher
