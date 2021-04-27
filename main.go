package main


import (
	"bufio"
	"encoding/json"
	"fmt"
	"net"
	"os"
	"strings"
)

/*
1) вся инфа заполнялась бы в текстовый файл (X)
2) Использовать ли маршрутизацию (может быть сложно из-за цикла) или записать все адреса, т.к. они известны и просто отправить вручную
точнее открытые ключи, как вид авторизации (использовав полносвязную топологию)
*/

type Node struct {
	Connections map[string]bool//(маппинг всех пользователей)
	Address Address
}

type Address struct {
	IPv4 string
	Port string
}

type Package struct {
	To string
	From string
	Data string
}

func init()  {
	if len(os.Args) != 2 {
		panic("len args != 2")
	}
}

var addr = [...]string{
	":8080",
	":7070",
	":6060",
	":5050",
}


func main()  {
	fmt.Println("Hello, in this program you can use these commands: /exit, /connect, /network")
	NewNode(os.Args[1]).Run(handleServer, handleClient)//создание ноды (доюавляем указанный нами адрес)
}

//ipv4:port
//127.0.0.1:8080
func NewNode(address string) *Node{//функция по созданию ноды (адрес -> структура)
	splted := strings.Split(address, ":")//разюиваем
	if len(splted) != 2{
		return nil
	}
	check := splted[1]
	n := 0
	for i := 0; i < len(addr)-1; i++{
		if (":"+check) == addr[i]{
			n++
		}
	}
	if n != 1{
		panic("We can`t find this port in our database!")
	}
	return &Node{
		Connections: make(map[string]bool),
		Address:Address{
			IPv4: splted[0],
			Port: ":"+splted[1],
		},
	}
}

func (node *Node) Run (handleServer func(*Node), handleClient func(*Node)) {//привязан к ноде
	go handleServer(node)
	handleClient(node)
}

func handleServer(node *Node)  {//только принимаем данные
	listen, err := net.Listen("tcp", "0.0.0.0" + node.Address.Port)//прослушивание и приме запросов
	if err != nil{
		panic("listen error")
	}
	defer listen.Close()
	for  {
		conn, err := listen.Accept()//принятие соединения
		if err != nil{
			break
		}
		go handleConnection(node, conn)
	}
}

func handleConnection(node *Node, conn net.Conn){
	defer conn.Close()
	var (
		buffer = make([]byte, 512)//буффер
		message string
		pack Package
	)
	for {
		length, err := conn.Read(buffer)//читаем и заполняем
		if err != nil{
			break
		}
		message += string(buffer[:length])//к сообщ добавляем буффер
	}
	err := json.Unmarshal([]byte(message), &pack)//раскодировка
	if err != nil{
		return
	}
	node.ConnectTo([]string{pack.From})//подключились (смотрим от кого)
	fmt.Println(pack.Data)
}

func handleClient(node *Node)  {//только отправляем данные
	f, err := os.OpenFile("blockchain.txt", os.O_APPEND|os.O_WRONLY, 0600)
	if err != nil {
		panic(err)
	}
	defer f.Close()
	for{
		message := InputString()//вводим команду или сообщение
		s := message[0:len(message)-1]
		splited := strings.Split(s, " ")
		switch splited[0] {
		case "/exit": {//выход
			os.Exit(0)
		}
		case "/connect": {//подключаемся (список адресов)
			for i := 0; i < len(addr); i++{
				splt := strings.Split(addr[i], " ")
				node.ConnectTo(splt[0:])
			}
			WriteInFile(f, s)
		}
		case "/network": {
			WriteInFile(f, s)
			node.PrintNetwork()
		}
		default: {//отправляем всем
			WriteInFile(f, s)
			node.SendMessageToAll(s)
		}
		}
	}
}

func (node *Node) PrintNetwork(){
	for addr := range node.Connections{
		fmt.Println("|",addr)
	}
}

func (node *Node) ConnectTo(addresses []string)  {
	for _, addr := range addresses{//перебираем все адреса
		node.Connections[addr] = true//заносим в маппинг
	}
}

func (node *Node) SendMessageToAll (message string)  {
	var new_pack = Package{
		From: node.Address.IPv4 + node.Address.Port,//наш адрем
		Data: message,//сообщение
	}
	for addr := range node.Connections{//перебираем адреса
		new_pack.To = addr//получатель - адрес
		node.Send(&new_pack)
	}
}

func (node *Node) Send(pack *Package)  {
	conn, err := net.Dial("tcp", pack.To)//кому отправляем (делаем запрос на выбранный адрес)
	if err != nil{
		delete(node.Connections, pack.To)
		return
	}
	defer conn.Close()
	json_pack,_ := json.Marshal(*pack)//представляем в json формате
	conn.Write(json_pack)
}

func InputString()  string{
	msg, _ := bufio.NewReader(os.Stdin).ReadString('\n')
	return strings.Replace(msg, "\n", "", -1)
}


func WriteInFile(f *os.File, s string) {
	_, err := f.WriteString(s)
	if err != nil{
		fmt.Println("Can`t write to file")
	}
}