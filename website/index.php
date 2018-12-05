<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="favicon.ico">

    <title>COMP 477 2018- FLUID SIMULATION</title>

    <!-- Bootstrap core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="css/starter-template.css" rel="stylesheet">
      <link href="http://allfont.net/allfont.css?fonts=world-of-water" rel="stylesheet" type="text/css" />
      <link href="css/style.css" rel="stylesheet">
      
      <style>
       body, html {
    height: 100%;
    padding:0;
    margin: 0;
    font-size: 20px;
}

section {
    /* The image used */
    background-image: url("img/bg1.jpg");

    /* Full height */
    height: 100%; 
    

    /* Center and scale the image nicely */
    background-position: top center;
    background-repeat: no-repeat;
    background-size: cover;
          }
          
          section .title{
              width: 100%;
              padding-top: 35%;
              font-size: 90px;
              font-family: 'World of Water', arial;
              text-shadow: 4px 4px 4px #aaa;
             text-align:center;
          }
          
          
          
          .gconcept{
              padding-top: 200px;
              padding-bottom: 200px;
              width: 100%;
              height: auto;
              background-color:#fff;
              
          }
          .gconcept h1{
              text-align: center;
          }
          
          
          .row p{
              margin: 20px;
              padding: 20px;
          }
      </style>
  </head>

  <body>

    <nav class="navbar navbar-expand-md navbar-dark bg-dark fixed-top">
      <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarsExampleDefault" aria-controls="navbarsExampleDefault" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
      </button>

      <div class="collapse navbar-collapse" id="navbarsExampleDefault">
        <ul class="navbar-nav mr-auto">
          <li class="nav-item active">
            <a class="nav-link" href="#">Home <span class="sr-only">(current)</span></a>
          </li>
          
        </ul>
        
      </div>
    </nav>

     <section>
      
         <h1 class="title">Fluid Simulation</h1>
      
      </section>
      
      <div class="container-fluid ">
        <div class="row">
            <div class="col-md-8 gconcept">
          <h1 class="display-2" style="margin-top: 20px; margin-bottom: 100px;color: #1C98DE;">GENERAL CONCEPT</h1>
                
                <div style="width: 100%; margin-top:0px; text-align: center;">
                    <img src="img/brain.png" style="width: 300px; height: auto;">
                
                </div>
                
            </div>
            <div class="col-md-4 gconcept">
                <p style="width: 100%; padding-top:100px; padding-bottom: 30px; margin-left: 0px; padding-right: 40px; text-align: left; font-size: 25px; line-height: 1.6;">
                    The goal of this project is to create a realistic and accurate fluid simulation, using efficient algorithms which can allow for a large number of particles. We will be using a particle system to implement smoothed particle hydrodynamics (SPH) interpolation. As we have limited time and no experience on the subject, exploring particle fluid simulation is much more suited than grid based solutions. The project will be developed using C++, OpenGl and any other useful libraries that may come up during the research period.
                </p>
                
          </div>
        </div>
          
          <div class="row">
          <div style="background-color: #FFBF69;">
          <div class="row" style="margin-top: 90px; width: 100%; height: 900px; background-image: url('img/fist.png');  background-position: center; background-repeat: no-repeat; background-size: cover;" >
          
            <h1 class="display-1" style="color: #fff; width: 100%;height: 200px; padding-top: 300px;text-align: right;">MOTIVATION  </h1>
              
          </div>
              
             
          
          
          <div class="row ">
       
                <p style="width: 100%; padding-top:20px; margin-bottom: 150px; padding-bottom: 30px; margin-left: 80px; margin-right: 80px; text-align: left; font-size: 25px; line-height: 1.6;">
	Out of all the topics that we had considered, fluid simulation was one that appealed to us. From our understanding, it is a topic that has been researched extensively and there are a variety of resources to draw information from. Originally, we were curious to explore this topic by simulating the physics property of oobleck (a particular mixing ratio with cornstarch and water mixture) solution. After some discussion, we concluded that it is strategically better to study basic fluid simulation through a conducted research, attempt to recreate the implementation and only at the end will we try to add more if time permits.
                </p>
            
        </div>
          
      </div>
              
              </div>
         
          <div class="row">
              <h1 class="display-2" style="margin-top: 80px; margin-bottom: 100px; color: #1C98DE;">Objectives & Milestones</h1>
              
               </div>
            
              
              <div class="row">
                
                  <div class="col-md-4">
                  
                  <p style="width: 100%; padding-top:20px; margin-bottom: 150px; padding-bottom: 30px; margin-left: 40px; margin-right: 40px; text-align: left; font-size: 25px; line-height: 1.6;">
                    The goal of this project is to create a realistic and accurate fluid simulation, using efficient algorithms which can allow for a large number of particles. We will be using a particle system to implement smoothed particle hydrodynamics (SPH) interpolation. As we have limited time and no experience on the subject, exploring particle fluid simulation is much more suited than grid based solutions. The project will be developed using C++, OpenGl and any other useful libraries that may come up during the research period.
                </p>

                  </div>
                  
                  <div class="col-md-8">
                    <img src="img/steps.png" style="width: 100%; height; auto; margin-bottom: 0px; padding-top: 60px;">
                  
                  </div>
                  
                                  
            </div>
          
          <div class="row">
             <div style="background-color: #CBF3F0; width:100%; margin-top: 80px; margin-bottom: 0px;">
           <h1 class="display-2" style="margin-top: 80px; margin-bottom: 60px;text-align:center;">The Research</h1>
          
                 <div class="row">
                 <p style="padding-top:20px;  padding-bottom: 30px; margin-left: 40px; margin-right: 40px; text-align: left; font-size: 25px; line-height: 1.6;">
                    Through the beginning of the process, we knew that we must determine what the component of our water particle will consist of. It is from this decision that will influence our implementation method later on. In the following image, we decided to base off our water particle traits from the SPH Fluids in Computer Graphics research paper.
                </p>
                 </div>
                 
                 <div class="row">
                     <div style="width: 500; text-align: center;">
                 <img src="img/water%20particle.png" style="width: 500px; height: auto; ">
                 </div>

                 </div>
                               
              </div>
          </div>
          
          <div class="row">
             <div style="background-color: #FFF; width:100%;">
                 <div class="row">
                 <p style="padding-top:20px;  padding-bottom: 30px; margin-left: 40px; margin-right: 40px; text-align: left; font-size: 25px; line-height: 1.6;">
                    From that, we began to build a data structure to support the entire system. Through various methods that could have been implemented, a custom Octree data structure was used to get the neighbors of a given particle, using the kernel radius. Some damping was also included to deal with collisions with the boundaries. The integration method implemented was explicit Euler, hence giving some unwanted results due to inaccuracy. We used an older version of Open GL (2.1), the Eigen library and Intel’s TBB, to parallelize some of the code and improve performance.. Once the data structure has been set, we simply just followed the research and it's given algorithms on implementing SPH. During our implementation process, we did encounter multiple situations where our code wasn't running as well as we had intended. The major one was Naiver-Stokes and the algorithm for SPH.
                </p>
                 </div>
                 
                 <div class="row">
                     <div style="width: 100%; text-align: center; margin-top: 60px;">
                 <img src="img/Navier.png" style="width: 500px; height: auto; ">
                 </div>

                 </div>
                 
                 <div class="row">
                     <div style="width: 100%; text-align: center;margin-top: 60px;">
                 <img src="img/SPH%20State%20Equ.png" style="width: 500px; height: auto; ">
                 </div>

                 </div>
                 
                 <div class="row">
                     <div style="width: 100%; text-align: center;margin-top: 60px;">
                 <img src="img/SPH%20Splitting.png" style="width: 500px; height: auto; ">
                 </div>

                 </div>
                 
                 <div class="row">
                 <p style="width: 100%; padding-top:20px;  padding-bottom: 30px; margin-left: 40px; margin-right: 40px; text-align: center; font-size: 25px; line-height: 1.6;">
                    There is also the viscosity of the water particles along with the influence of pressure.
                </p>
                 </div>
                     
                <div class="row">
                     <div style="width: 100%; text-align: center;margin-top: 20px;">
                 <img src="img/EOS%20Splitting.png" style="width: 500px; height: auto; ">
                 </div>
                     
                 </div>
                 
                  <div class="row">
                 <p style="width: 100%; padding-top:20px;  padding-bottom: 30px; margin-left: 40px; margin-right: 40px; text-align: center; font-size: 25px;">
                    In the following video, you will see that our problem could have been a variety of things such as lack of pressure. While all of the physics were implemented, some of them are not working properly. The variables that influence the simulation correctly are gravity, viscosity and forces. However, we were not able to make the fluid incompressible, which means that the density within the WaterParticle does not remain constant. This is why we can observe that the water particles fall flat and collide into each other, ending up at the corners rapidly.

                     
                </p>
                 </div>
                 
                 <div class="row">
                     <div style="width: 100%; text-align: center;margin-top: 20px;">
                 <video width="500px" height="auto" controls>
                    <source src="vid/inProgress.mp4" type="video/mp4">
                 </video>
                 </div>           
              </div>
                 
                
                  <div class="row">
                 <p style="width: 100%; padding-top:20px;  padding-bottom: 30px; margin-left: 40px; margin-right: 40px; text-align: center; font-size: 25px;">
                     Though we have implemented our base code for fluid simulation, we didn’t have enough time to include any properties that will simulate the physics property of oobleck.

                </p>
                 </div>
                 
                 <div class="row">
                     <div style="width: 100%; text-align: center;margin-top: 20px;">
                 <video width="500px" height="auto" controls>
                    <source src="vid/final.mp4" type="video/mp4">
                 </video>
                 </div>           
              </div>
                 
                  <div class="row">
                 <p style="width: 100%; padding-top:20px;  padding-bottom: 30px; margin-left: 40px; margin-right: 40px; text-align: center; font-size: 25px;">
                      Though we have implemented our base code for fluid simulation, we didn’t have enough time to include any properties that will simulate the physics property of oobleck.
                </p>
                 </div>
                 
               
                 
          </div>
          </div>
          
        
           
           
            
            
            <div class="row">
             <div style="background-color: #CBF3F0; width:100%; margin-top: 80px; padding-bottom: 80px;">
           <h1 class="display-2" style="margin-top: 80px; margin-bottom: 60px;text-align:center;">References</h1>
          
                 <div class="row">
                <ul style="list-style-type: none;
    margin: 40px;
    padding: 20px;
                           font-size: 30px;
                           line-height: 1.6;">
                <li><a href="https://cg.informatik.uni-freiburg.de/publications/2014_EG_SPH_STAR.pdf?fbclid=IwAR2xlux4eD5pWR8rODokEwF11S38K0WS_FoKHAHqd-zMQYcvywTRxDCSveo">Fluid Simulation For Computer Graphics</a></li>
                <li><a href="https://cg.informatik.uni-freiburg.de/intern/seminar/gridFluids_fluid-EulerParticle.pdf">Fluid Simulation For Computer Graphics: A Tutorial in Grid Based and Particle Based Methods</a></li>
                <li><a href="https://bigtheta.io/2017/07/08/implementing-sph-in-2d.html?fbclid=IwAR1UCaqDkMsS9L2ZY7056G4tzVhfSKccrjfbwxqSNhEHlt_WYlfy5PxwUi8">Implementing SPH in 2D</a></li>
                <li><a href="http://eigen.tuxfamily.org/index.php?title=Main_Page">Eigen Library</a></li>
                <li><a href="https://pdfs.semanticscholar.org/fbc1/b9c2a3438c79a1ef8684bc87ac5e2149643e.pdf">Boundary Handling</a></li>
                    
                    
                    
                    <li><a href="http://matthias-mueller-fischer.ch/publications/sca03.pdf">Particle-Based Fluid Simulation for Interactive Applications
</a></li>

                     <li><a href="https://nccastaff.bournemouth.ac.uk/jmacey/MastersProjects/MSc16/13/thesis.pdf">OpenGL GPU Features and SPH Fluid

</a></li>
                    
                     <li><a href="https://github.com/Maarten-vd-Sande/smoothed-particle-hydrodynamics
                    ">Smoothed Particle Hydrodynamics Reference

</a></li>
                    <li><a href=" https://www.youtube.com/watch?v=SQPCXzqH610
                    
                    ">Smoothed Particle Hydrodynamics Lecture

</a></li>
         
                    <li><a href=" https://www.youtube.com/watch?v=tAXHCAEgSuE
         ">SPH Lecture

</a></li>           
                    
                   
                   
            </ul>
                     
                     
                     
                 </div>
                 
                 
                               
              </div>
          </div>
              
         
    

      
      
    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="js/jquery-3.3.1.slim.min.js" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
    <script>window.jQuery || document.write('<script src="js/jquery-slim.min.js"><\/script>')</script>
    <script src="js/jquery-3.3.1.slim.min.js"></script>
    <script src="js/jquery.ripples-min.js"></script>
    <script src="js/bootstrap.min.js"></script>
      <script type="text/javascript">
        $(document).ready(function(){
            $('section').ripples({
                dropRadius: 12,
                pertubance: 0.05,
            });
        });
      </script>
     
  </body>
</html>
