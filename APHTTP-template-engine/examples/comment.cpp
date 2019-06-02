#include "comment.h"

void comment::set_reply(string _content)
{
  replies.push_back(_content);
}


void comment::show_replies(int id)
{
  for(int i = 0;i < replies.size();i++)
    cout<<id<<"."<<i + 1<<". "<<replies[i]<<endl;
}
