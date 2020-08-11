import {Router} from "express";
import ConnectionController from "./controllers/ConnectionController";

const connectionController = new ConnectionController;



const routes = Router();

routes.get("/connection", connectionController.set);


export default routes;