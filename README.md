# 🐚 Minishell  
---

## 🎯 Projenin Amacı  
Bu proje, **Bash**’in temel işlevlerini taklit eden basit bir shell (**komut yorumlayıcı**) geliştirmeyi amaçlar.  
Minishell ile:  
- 👩‍💻 Kullanıcıdan komut alıp çalıştırmayı,  
- 🔄 **Process** ve **file descriptor** yönetimini,  
- 📂 Ortam değişkenlerini ve yönlendirmeleri,  
- ⛓️ **Pipes** ve **redirections** (>, <, >>, <<),  
- 🛠️ **Yerleşik (built-in) komutları** (cd, echo, pwd, export, unset, env, exit)  
uygulayarak bir **mini Bash** deneyimi oluşturuyoruz.  

---

## 🧩 Özellikler  
✔️ Prompt görüntüleme ve komut bekleme  
✔️ Çalışan **history** desteği  
✔️ PATH üzerinden doğru executable bulma  
✔️ Tek tırnak `'` ve çift tırnak `"` desteği  
✔️ Çevresel değişkenler `$VAR` ve `$?` çözümleme  
✔️ **Redirectionlar** (<, >, >>, <<)  
✔️ **Pipe (|)** desteği  
✔️ Sinyal yakalama (ctrl-C, ctrl-D, ctrl-\)  
✔️ Yerleşik komutlar:  
   - `echo [-n]`  
   - `cd`  
   - `pwd`  
   - `export`  
   - `unset`  
   - `env`  
   - `exit`  

---

## 🧠 Neler Öğrendim?  
Bu proje bana şunları kattı:  

- ⚙️ **Process yönetimi** (fork, execve, wait)  
- 🔄 **Pipe** ve **redirection** mantığı  
- 🔑 **File descriptor** kullanımı  
- 🛡️ **Signal handling** (ctrl-C, ctrl-D, ctrl-\)  
- 🌍 **Environment variables** yönetimi  
- 📝 **Parser** ve **lexer** yazma deneyimi  
- ♻️ Bellek yönetiminde dikkatli olmanın önemi  
