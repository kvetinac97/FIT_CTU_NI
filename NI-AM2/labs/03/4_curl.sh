curl --header "Content-Type: application/json" \
  --request POST \
  --data '{"username":"admin"}' \
  http://localhost:8080/login
echo ""

curl -X GET \
  -H 'Authorization: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpYXQiOjE2NzkzMzA2Mjk5OTcsImlzcyI6InNvbWVib2R5IiwidXNlcm5hbWUiOiJzb21lYm9keSIsImV4cGlyYXRpb24iOiIyMDIzLTAzLTIwVDE2OjQ4OjQ5Ljk5N1oifQ==.zeyfWg/N54eQlewmkv2iD7e+5vH/z0vMhmBUKJ1Dl7Y=' \
  http://localhost:8080/data
echo ""

curl -X GET http://localhost:8080/hmac
echo ""