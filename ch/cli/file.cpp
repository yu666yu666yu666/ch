#include "cli.hpp"

void fsendfile(){
    fsendfile1 p;
    std::string id;
    int m = 0;
    yesorno tt;
    flist1 p1;
    std::vector<ffriend> t;
    std::string json_str;
    json j;
    p1 = {STATE_FLIST1,myid};
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<ffriend>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& js : j){
        ffriend t1;
        t1.ustate = js["1"].get<int>();
        t1.id = js["2"].get<std::string>();
        t.push_back(t1);
    }
    for(const auto& ffriend : t){
        std::cout << '\n' << "id:" << ffriend.id << ",state:";
        if(ffriend.ustate == STATE_ON)
            std::cout << "在线";
        else
            std::cout << "离线";
    }
        std::cout << '\n' << "输入你要发送文件的用户的id:";
        std::getline(std::cin,id);
        for(const auto& str : t){
            if(id == str.id){
                m = 1;
                p.id = id;
                std::cout<< '\n' << "要发送的文件(路径):(输入“退出退出退出”离开此界面)";
                std::getline(std::cin,p.filename);
                while (!std::filesystem::exists(p.filename)){
                    if(p.filename == "退出退出退出")
                        return;
                    std::cout << "该文件不存在，请重新输入(输入“退出退出退出”离开此界面)" << std::endl;
                    std::getline(std::cin,p.filename);
                }
                break;
            }
        }
        if(m){
            int file = open(p.filename.c_str(), O_RDONLY);
            if (file < 0){
                std::cerr << "Failed to open file" << std::endl;
                return;
            }
            struct stat file_stat;
            fstat(file, &file_stat);
            p.filesize = file_stat.st_size;
            std::filesystem::path path(p.filename);
            p.filename = path.filename().string();
            p.cid = myid;
            p.state = STATE_FSENDFILE1;
            j = {{"1", p.state},{"2", p.cid},{"3", p.id},{"4", p.filename},{"5", p.filesize}};
            json_str = j.dump();
            fa(json_str);
            sleep(3);
            off_t off = 0;
            ssize_t bytes = 0;
            while (off < file_stat.st_size){
                bytes = sendfile(client_socket, file, &off, file_stat.st_size - off);
                if (bytes < 0){
                    std::cerr <<"sendfile err" << '\n';
                    break;
                }
                 std::cout << p.filename << ": " << (int)(((float)off / file_stat.st_size) * 100) << "%" << std::flush;
            }
            std::cout  << '\n'<<file_stat.st_size << '|'<< off;
            std::cout << std::endl;
            close(file);    
        }
        else
            std::cout << '\n' << "他不是你的好友!";
    std::cout <<std::endl;
}

void frecvfile(){
    std::string json_str;
    std::string id;
    frecvfile1 p;
    frecvfile2 t;
    frecvfile3 pp;
    frecvfile4 tt;
    std::string filename;
    json j;
        std::cout << '\n' << "你要接收谁发来的文件?输入id:";
        std::getline(std::cin,id);
        p = {STATE_FRECVFILE1,myid,id};
        if(id == myid){
            std::cout << '\n' << "不要输入自己的id!";
            return;
        }
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.id}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<fhistory2>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        t.filename = j["2"].get<std::vector<std::string>>();
        if(t.state == STATE_YES){
            for(const auto& str : t.filename){
            std::cout << '\n' << str;
            }
            std::cout << std::endl;
            std::cout <<"选择你要接收的文件:(输入“退出退出退出”离开接收界面)";
            while(1){
                std::getline(std::cin,filename);
                if(filename == "退出退出退出")
                    break;
                if(std::count(t.filename.begin(),t.filename.end(),filename) <= 0){
                    std::cout << '\n' << "列表中没有该文件!重新输入:";
                    continue;
                }
                j = {{"1", STATE_FRECVFILE2},{"2", myid},{"3", id},{"4", filename}};
                json_str = j.dump();
                fa(json_str);

                json_str = shou();
                j = json::parse(json_str);
                tt.filesize = j["1"].get<long long int>();

                std::cout << "开始接收文件" << std::endl;
                std::string filename1;
                std::filesystem::path dirpath = "../client/";
                if (!std::filesystem::exists(dirpath)){
                    std::filesystem::create_directories(dirpath);
                    std::cout << "正在创建文件" << std::endl;
                }
                
                filename1 = dirpath.c_str() + filename;
                FILE *fp = fopen(filename1.c_str(), "wb");
                if (fp == nullptr) 
                    std::cerr << "Failed to open file" << std::endl;  
 
                int len;
                char buffer[10240];
                off_t off = 0;
                sleep(5);
                while (off < tt.filesize){
                    len = recv(client_socket, buffer, (off + 10240 > tt.filesize) ? tt.filesize - off : 10240, 0);
                    if (len <= 0){
                        if (len < 0)
                            perror("recv");
                    }
                    fwrite(buffer, 1, len, fp);
                    off += len;   
                    std::cout  << filename << ": " << (int)(((float)off / tt.filesize) * 100) << "%" << std::flush;              
                }
                std::cout << std::endl;
                fclose(fp);
                std::cout << off << std::endl;
                if (off == tt.filesize){
                    std::cout << "接收成功" << std::endl;
                }
                else
                    std::cerr << "不全" << std::endl;
            }
        }
        else if(t.state == STATE_NOFRIEND)
            std::cout << '\n' << "他不是你的好友!";
        else if(t.state == STATE_FNOEXIT)
            std::cout << '\n' << "用户不存在!";
        else 
            exit(1);
}

void gsendfile(){
    gsendfile1 p;
    std::string gid;
    int m = 0;
    yesorno tt;

    glist1 p1;
    std::vector<fgroup> t;
    std::string json_str;
    json j;
    p1 = {STATE_GLIST1,myid};
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<fgroup>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& js : j){
        fgroup t1;
        t1.gid = js["1"].get<std::string>();
        t1.g_leader = js["2"].get<std::string>();
        t1.manager = js["3"].get<std::vector<std::string>>();
        t1.member = js["4"].get<std::vector<std::string>>();
        t.push_back(t1);
    }
    for(const auto& fgroup : t){
        std::cout << '\n' << "gid:" << fgroup.gid;
    }
    std::cout << std::endl;

        std::cout << '\n' << "输入你要发送文件的群的gid:";
        std::getline(std::cin,gid);
        for(const auto& str : t){
            if(gid == str.gid){
                m = 1;
                p.gid = gid;
                std::cout<< '\n' << "要发送的文件(路径):(输入“退出退出退出”离开此界面)";
                std::getline(std::cin,p.filename);
                while (!std::filesystem::exists(p.filename)){
                    if(p.filename == "退出退出退出")
                        return;
                    std::cout << "该文件不存在，请重新输入(输入“退出退出退出”离开此界面)" << std::endl;
                    std::getline(std::cin,p.filename);
                }
                break;
            }
        }
        if(m){
            int file = open(p.filename.c_str(), O_RDONLY);
            if (file < 0){
                std::cerr << "Failed to open file" << std::endl;
                return;
            }
            struct stat file_stat;
            fstat(file, &file_stat);
            p.filesize = file_stat.st_size;
            std::filesystem::path path(p.filename);
            p.filename = path.filename().string();
            p.cid = myid;
            p.state = STATE_GSENDFILE1;
            j = {{"1", p.state},{"2", p.cid},{"3", p.gid},{"4", p.filename},{"5", p.filesize}};
            json_str = j.dump();
            fa(json_str);
            sleep(3);
            off_t off = 0;
            ssize_t bytes = 0;
            while (off < file_stat.st_size){
                bytes = sendfile(client_socket, file, &off, file_stat.st_size - off);
                if (bytes < 0){
                    std::cerr <<"sendfile err" << '\n';
                    break;
                }
                 std::cout << p.filename << ": " << (int)(((float)off / file_stat.st_size) * 100) << "%" << std::flush;
            }
            std::cout << std::endl;
            close(file);    
        }
        else
            std::cout << '\n' << "你不在该群!";
    std::cout <<std::endl;
}

void grecvfile(){
    std::string json_str;
    std::string gid;
    grecvfile1 p;
    grecvfile2 t;
    grecvfile3 pp;
    grecvfile4 tt;
    std::string filename;
    json j;
        std::cout << '\n' << "你要接收哪个群的文件?输入gid:";
        std::getline(std::cin,gid);
        p = {STATE_GRECVFILE1,myid,gid};
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.gid}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<fhistory2>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        t.filename = j["2"].get<std::vector<std::string>>();
        if(t.state == STATE_YES){
            for(const auto& str : t.filename){
            std::cout << '\n' << str;
            }
            std::cout << std::endl;
            std::cout <<"选择你要接收的文件:(输入“退出退出退出”离开接收界面)";
            while(1){
                std::getline(std::cin,filename);
                if(filename == "退出退出退出")
                    break;
                if(std::count(t.filename.begin(),t.filename.end(),filename) <= 0){
                    std::cout << '\n' << "列表中没有该文件!重新输入:";
                    continue;
                }
                j = {{"1", STATE_GRECVFILE2},{"2", myid},{"3", gid},{"4", filename}};
                json_str = j.dump();
                fa(json_str);

                json_str = shou();
                j = json::parse(json_str);
                tt.filesize = j["1"].get<long long int>();

                std::cout << "开始接收文件" << std::endl;
                std::string filename1;
                std::filesystem::path dirpath = "../client/";
                if (!std::filesystem::exists(dirpath)){
                    std::filesystem::create_directories(dirpath);
                    std::cout << "正在创建文件" << std::endl;
                }
                filename1 = dirpath.c_str() + filename;
                FILE *fp = fopen(filename1.c_str(), "wb");
                if (fp == nullptr) 
                    std::cerr << "Failed to open file" << std::endl;  
 
                int len;
                char buffer[10240];
                off_t off = 0;
                sleep(5);
                while (off < tt.filesize){
                    len = recv(client_socket, buffer, (off + 10240 > tt.filesize) ? tt.filesize - off : 10240, 0);
                    if (len <= 0){
                        if (len < 0)
                            perror("recv");
                    }
                    fwrite(buffer, 1, len, fp);
                    off += len;   
                    std::cout  << filename << ": " << (int)(((float)off / tt.filesize) * 100) << "%" << std::flush;              
                }
                std::cout << std::endl;
                fclose(fp);
                std::cout << off << std::endl;
                if (off == tt.filesize){
                    std::cout << "接收成功" << std::endl;
                }
                else
                    std::cerr << "不全" << std::endl;
            }
        }
        else if(t.state == STATE_NOGROUP)
            std::cout << '\n' << "你不在该群!";
        else if(t.state == STATE_GNOEXIT)
            std::cout << '\n' << "群聊不存在!";
        else 
            exit(1);
}