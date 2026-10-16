//
// Created by motong on 2026/10/16.
//
#include <pangolin/pangolin.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

static const std::string trajectory_file = "../trajectory.txt";
static void DrawTrajectory(std::vector<Eigen::Isometry3d,Eigen::aligned_allocator<Eigen::Isometry3d>>);

int main(int argc , char** argv) {
    std::vector<Eigen::Isometry3d,Eigen::aligned_allocator<Eigen::Isometry3d>> poses;
    std::ifstream fin(trajectory_file);
    if (!fin) {
        std::cout << "cannot find trajectory file at " << trajectory_file << std::endl;
    }
    while (!fin.eof()) {
        double time,tx,ty,tz,qx,qy,qz,qw;
        fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        Eigen::Isometry3d T_wr(Eigen::Quaterniond(qw,qx,qy,qz));
        T_wr.pretranslate(Eigen::Vector3d(tx,ty,tz));
        poses.push_back(T_wr);
    }
    std::cout << "read total " << poses.size() << " pose entries" << std::endl;

    DrawTrajectory(poses);
    return 0;
}

void DrawTrajectory(std::vector<Eigen::Isometry3d, Eigen::aligned_allocator<Eigen::Isometry3d> > poses) {
    //下面的gl开头的是opengl的函数，pangolin是对opengl的封装
    pangolin::CreateWindowAndBind("Trajectory Viewer", 1024, 768);
    //创建并绑定一个openGL窗口，名字为Trajectory Viewer，大小为1024*768
    glEnable(GL_DEPTH_TEST);
    //设置opengl渲染时对深度信息进行处理，画三维的图像时需要打开这个功能
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //开启透明混合功能，并指定混合方式，新颜色按照透明度叠加到已有画面上，
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)表示源颜色的alpha值决定了源颜色的透明度，
    //1-alpha决定了目标颜色的透明度，GL_ONEMINUS_SRC_ALPHA是背景权重，这一部分可以照抄

    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(1024,768,500,500,512,389,0.1,1000),
        pangolin::ModelViewLookAt(0,-0.1,-1.8,0,0,0,0.0,-1.0,0.0)
    );
    //创建一个相机对象s_cam，OpenGlRenderState打包了这个虚拟相机的所有参数
    //ProjectionMatrix（宽，高，fx,fy,cx,cy,近，远）fx/fy是焦距（像素单位），cx/cy是主点坐标，近远表示视点的前后范围
    //ModelViewLookAt是视图矩阵，九个数表示三个点，相机位置，看向的点，向上的方向

    pangolin::View &d_cam = pangolin::CreateDisplay()
        .SetBounds(0.0,1.0,0.0,1.0,-1024.0f/768.0f)
        .SetHandler(new pangolin::Handler3D(s_cam));
    //创建一个视图对象d_cam，CreateDisplay()创建一个显示窗口，SetBounds设置显示窗口的范围（上，下，左，右，宽高比）
    //SetHandler设置鼠标操作的处理器

    while (pangolin::ShouldQuit() == false) {
        //渲染流程，只要没有关闭窗口，就一直循环渲染
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //清除颜色和深度缓存，把上一帧的图像清除掉，准备渲染新的一帧
        d_cam.Activate(s_cam);
        //激活相机，告诉opengl接下来要用这个相机渲染图像
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        //设置背景颜色为白色，前三个参数是RGB，最后一个参数是透明度，严格来说应该先设置背景颜色，再清除颜色缓存，但这里顺序颠倒了，没关系

        glLineWidth(2);
        //设置线宽为2个像素
        for (auto & pose : poses) {
            Eigen::Vector3d Ow= pose.translation();
            //位姿的位置 = 自身原点在世界系的位置
            Eigen::Vector3d Xw = pose * (0.1 * Eigen::Vector3d(1,0,0));
            // 自身 x 轴 0.1 长，变换到世界系
            Eigen::Vector3d Yw = pose * (0.1 * Eigen::Vector3d(0,1,0));
            Eigen::Vector3d Zw = pose * (0.1 * Eigen::Vector3d(0,0,1));
            //这里三个轴沿自身的方向各画一条线，长度为0.1，变换到世界系后就可以画出来了
            glBegin(GL_LINES);
            //画线，GL_LINES表示画线段，glBegin和glEnd之间的代码就是画线段的顶点坐标
            glColor3f(1,0,0);
            glVertex3d(Ow[0],Ow[1],Ow[2]);
            glVertex3d(Xw[0],Xw[1],Xw[2]);
            glColor3f(0,1,0);
            glVertex3d(Ow[0],Ow[1],Ow[2]);
            glVertex3d(Yw[0],Yw[1],Yw[2]);
            glColor3f(0,0,1);
            glVertex3d(Ow[0],Ow[1],Ow[2]);
            glVertex3d(Zw[0],Zw[1],Zw[2]);
            glEnd();
        }
        for (size_t i =0;i<poses.size();i++){
        glColor3f(0.0,0.0,0.0);
            glBegin(GL_LINES);
            auto p1 = poses[i],p2 = poses[i+1];
            glVertex3d(p1.translation()[0], p1.translation()[1], p1.translation()[2]);
            glVertex3d(p2.translation()[0], p2.translation()[1], p2.translation()[2]);
            glEnd();
        }
        pangolin::FinishFrame();
        //提交这一帧
        usleep(5000);
        //暂停5毫秒，控制渲染速度
    }
}
