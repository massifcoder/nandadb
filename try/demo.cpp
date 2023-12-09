// void flush_data(AVLTree &avlTree){
//     vector<SSTNode> result;
//     preOderTraversal(avlTree.root, result);
//     avlTree.root = nullptr;
//     ofstream out("samp.txt");
//     if(!out.is_open()){
//         cout<<"Error in opening file."<<endl;
//         return;
//     }
//     for(auto node : result){
//         out.write((char*)&node, sizeof(node));
//     }
//     out.close();
// }